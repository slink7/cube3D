#include <Windows.h>
#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int32_t int32;

#define SKIP_BYTES(mem, bytes_to_skip) (mem += bytes_to_skip)

int32 sh_abs(int32 number)  {
    return (number > 0) ? number : -1*number;
}

uint8 base_length_extra_bit[] = {
    0, 0, 0, 0, 0, 0, 0, 0, //257 - 264
    1, 1, 1, 1, //265 - 268
    2, 2, 2, 2, //269 - 273 
    3, 3, 3, 3, //274 - 276
    4, 4, 4, 4, //278 - 280
    5, 5, 5, 5, //281 - 284
    0           //285
};

uint32 base_lengths[] = {
    3, 4, 5, 6, 7, 8, 9, 10, //257 - 264
    11, 13, 15, 17,          //265 - 268
    19, 23, 27, 31,          //269 - 273 
    35, 43, 51, 59,          //274 - 276
    67, 83, 99, 115,         //278 - 280
    131, 163, 195, 227,      //281 - 284
    258                      //285
};

uint32 dist_bases[] = {
    /*0*/ 1, 2, 3, 4,    //0-3
    /*1*/ 5, 7,          //4-5
    /*2*/ 9, 13,         //6-7
    /*3*/ 17, 25,        //8-9
    /*4*/ 33, 49,        //10-11
    /*5*/ 65, 97,        //12-13
    /*6*/ 129, 193,      //14-15
    /*7*/ 257, 385,      //16-17
    /*8*/ 513, 769,      //18-19
    /*9*/ 1025, 1537,    //20-21
    /*10*/ 2049, 3073,   //22-23
    /*11*/ 4097, 6145,   //24-25
    /*12*/ 8193, 12289,  //26-27
    /*13*/ 16385, 24577, //28-29
             0   , 0      //30-31, error, shouldn't occur
};

uint32 dist_extra_bits[] = {
    /*0*/ 0, 0, 0, 0, //0-3
    /*1*/ 1, 1,       //4-5
    /*2*/ 2, 2,       //6-7
    /*3*/ 3, 3,       //8-9
    /*4*/ 4, 4,       //10-11
    /*5*/ 5, 5,       //12-13
    /*6*/ 6, 6,       //14-15
    /*7*/ 7, 7,       //16-17
    /*8*/ 8, 8,       //18-19
    /*9*/ 9, 9,       //20-21
    /*10*/ 10, 10,    //22-23
    /*11*/ 11, 11,    //24-25
    /*12*/ 12, 12,    //26-27
    /*13*/ 13, 13,     //28-29
            0 , 0      //30-31 error, they shouldn't occur
};



struct sh_png_chunk {
    uint32 data_length;
    uint8 type[4];
    uint8  *data;
    uint32 crc32;
};

struct sh_zlib_block {
    uint8 cmf;
    uint8 extra_flags;
    uint8 *data;
    uint16 check_value;
};

struct sh_png_bit_stream {
    uint8 *data_stream;
    uint32 bit_buffer;
    uint32 bits_remaining;
};

enum sh_png_filters {
    sh_no_filter,
    sh_sub_filter,
    sh_up_filter,
    sh_avg_filter,
    sh_paeth_filter
};

uint8* sh_memalloc(uint32 bytes_to_allocate) {
    return (uint8 *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes_to_allocate);
}

uint8 sh_memfree(uint8 *mem_pointer) {
    return HeapFree(GetProcessHeap(), 0, (LPVOID) mem_pointer);        
}

void sh_memcpy(uint8 *from, uint8 *to, uint32 bytes_to_copy) { //copy some bytes from "from" to "to" 

    while(bytes_to_copy-- > 0) {
        *(to + bytes_to_copy) = *(from + bytes_to_copy);
    }
}

void sh_memset(uint8 *mem, uint8 value_to_use, uint32 bytes_to_set) {
    while(bytes_to_set-- > 0) {
        *mem++ = value_to_use;
    } 
}

uint16 sh_get_uint16be(uint8 *mem) {
    uint16 result = 0;
    for(uint32 i = 0; i < 2; ++i) {
        result <<= 8;
        result |= *(mem + i);
    }

    return result;
}


uint32 sh_get_uint32be(uint8 *mem) {
    uint32 result = 0;
    for(uint32 i = 0; i < 4; ++i) {
        result <<= 8;
        result |= *(mem + i);
    }

    return result;
}


uint8* sh_read_file(const char *file_name) {
    uint8 *result = NULL;
    HANDLE file = CreateFile(
            (const char *)file_name,
            GENERIC_READ,
            FILE_SHARE_READ,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0
            );

    DWORD size = GetFileSize(file, 0);

    result = sh_memalloc(size);
    ReadFile(file, (void *) result, size, 0, 0);
    CloseHandle(file);
    return result;
}

sh_png_chunk sh_png_read_chunk(uint8 *mem) {
    sh_png_chunk chunk = {};
    chunk.data_length = sh_get_uint32be(mem);
    SKIP_BYTES(mem, 4); //we move 4 bytes over because we read the length, which is 4 bytes

    *( (uint32 *)&chunk.type) = *((uint32 *) mem);
    SKIP_BYTES(mem, 4);

    chunk.data = sh_memalloc(chunk.data_length);

    sh_memcpy(mem, chunk.data, chunk.data_length);
    SKIP_BYTES(mem, chunk.data_length);

    chunk.crc32 = sh_get_uint32be(mem);

    return chunk;
}



sh_zlib_block sh_read_zlib_block(uint8 *mem, uint32 length) {
    sh_zlib_block zlib_block = {};
    zlib_block.cmf = *mem;
    SKIP_BYTES(mem, 1);

    zlib_block.extra_flags = *mem;
    SKIP_BYTES(mem, 1);

    //Length is the sum of all the data, we consumed two bytes, and the last two bytes are for the check value
    zlib_block.data = sh_memalloc(length - 2 - 2); //2 for cmf, and flag, 2 for check value

    //Remember we already skipped 2 bytes pointer wise, they were for the cmf and flag bytes
    sh_memcpy(mem, zlib_block.data, length - 2); 
    SKIP_BYTES(mem, length - 2);

    zlib_block.check_value = sh_get_uint16be(mem);

    return zlib_block;
}

void sh_png_get_bits(sh_png_bit_stream *bits, uint32 bits_required) {
    //this is an extremely stupid way to make sure the unsigned integer doesn't underflow, this is just a replacement for abs() but on unsigned integers.
    uint32 extra_bits_needed = (bits->bits_remaining > bits_required) ? (bits->bits_remaining - bits_required) : (bits_required - bits->bits_remaining);
    uint32 bytes_to_read = extra_bits_needed/8;

    //because the above is integer division, there is a possiblity of bits to be remaining, i.e: imagine extra_bits_needed is 14, if you do integer division by 8, you get 1, but an extra 6 bits remain 
    if(extra_bits_needed%8) { //do we have any remaining bits?
        bytes_to_read++; //if we do have extra bits they won't be more than 8 bits, so we will add one extra byte for those bits and we are good to go
    }

    for(uint32 i = 0; i < bytes_to_read; ++i) {
        uint32 byte = *bits->data_stream++;
        bits->bit_buffer |= byte << (i*8 + bits->bits_remaining); //we need to becareful to not overwrite the remaining bits if any
    }

    bits->bits_remaining += bytes_to_read*8;
}

uint32 sh_png_read_bits(sh_png_bit_stream *bits, uint32 bits_to_read) {

    uint32 result = 0;

    if(bits_to_read > bits->bits_remaining) {
        sh_png_get_bits(bits, bits_to_read);
    }

    for(uint32 i = 0; i < bits_to_read; ++i) {
        uint32 bit = bits->bit_buffer & (1 << i);
        result |= bit;
    }

    bits->bit_buffer >>= bits_to_read;
    bits->bits_remaining -= bits_to_read;

    return result;
}

uint32 sh_peak_bits_reverse(sh_png_bit_stream *bits, uint32 bits_to_peak) {
    if(bits_to_peak > bits->bits_remaining) {
        sh_png_get_bits(bits, bits_to_peak);
    }

    uint32 result = 0; //this could potentially cause problems, 
    for(uint32 i = 0; i < bits_to_peak; ++i) {
        result <<= 1;
        uint32 bit = bits->bit_buffer & (1 << i);
        result |= (bit > 0) ? 1 : 0;
    }

    return result;
}

int32 sh_png_paeth_predict(int32 a, int32 b, int32 c) { 
    int32 p = a + b - c;
    int32 pa = sh_abs(p - a);
    int32 pb = sh_abs(p - b);
    int32 pc = sh_abs(p - c);

    if(pa <= pb && pa <= pc) return a;
    if(pb <= pc) return b;
    return c;
}

uint8 sh_get_maximum_bit_length(uint8 *code_bit_lengths, uint32 len_of_array) {
    uint8 max_bit_length = 0;
    for(uint32 i = 0; i < len_of_array; ++i) {
        if(max_bit_length < code_bit_lengths[i]) {
            max_bit_length = code_bit_lengths[i];
        }
    }

    return max_bit_length;
}

void sh_get_bit_length_count(uint32 *code_count, uint8 *code_bit_length, uint32 bit_len_array_len) {
    for(uint32 i = 0; i < bit_len_array_len; ++i) {
        code_count[code_bit_length[i]]++;
    }
}

void sh_first_code_for_bitlen(uint32 *first_codes, uint32 *code_count, uint32 max_bit_length) {
    uint32 code = 0;
    for(uint32 i = 1; i <= max_bit_length; ++i) {
        code = ( code + code_count[i-1]) << 1; 

        if(code_count[i] > 0) {
            first_codes[i] = code;
        }
    }
}

uint32 sh_decode_huffman(sh_png_bit_stream *bits, uint32 *assigned_codes, uint8 *code_bit_lengths, uint32 assigned_code_length) {
    for(uint32 i = 0; i < assigned_code_length; ++i) {
        if(code_bit_lengths[i] == 0) continue;
        uint32 code = sh_peak_bits_reverse(bits, code_bit_lengths[i]);
        if(assigned_codes[i] == code) {
            bits->bit_buffer >>= code_bit_lengths[i];
            bits->bits_remaining -= code_bit_lengths[i];
            return i;
        }
    }

    return 0;
}

void sh_assign_huffman_code(uint32 *assigned_codes, uint32 *first_codes, uint8 *code_bit_lengths, uint32 len_assign_code) {
    for(uint32 i = 0; i < len_assign_code; ++i) {
        if(code_bit_lengths[i]) {
            assigned_codes[i] = first_codes[code_bit_lengths[i]]++;
        }
    }
}

uint32* sh_build_huffman_code(uint8 *code_bit_lengths, uint32 len_code_bit_lengths) {
    uint32 max_bit_length = sh_get_maximum_bit_length(code_bit_lengths, len_code_bit_lengths);

    uint32 *code_counts = (uint32 *)sh_memalloc(sizeof(uint32)*( max_bit_length + 1 ));
    uint32 *first_codes = (uint32 *)sh_memalloc(sizeof(uint32)*(max_bit_length + 1));
    uint32 *assigned_codes = (uint32 *)sh_memalloc(sizeof(uint32)*(len_code_bit_lengths));//we have to assign code to every element in the alphabet, even if we have to assign zero


    sh_get_bit_length_count(code_counts,  code_bit_lengths, len_code_bit_lengths);
    code_counts[0] = 0; //in the real world, when a code of the alphabet has zero bit length, it means it doesn't occur in the data thus we have to reset the count for the zero bit length codes to 0.

    sh_first_code_for_bitlen(first_codes, code_counts, max_bit_length);
    sh_assign_huffman_code(assigned_codes, first_codes, code_bit_lengths, len_code_bit_lengths);

    return assigned_codes;
}

uint8* sh_zlib_deflate_block(
        sh_png_bit_stream *bits,
        uint32 *literal_tree, uint8 *lit_code_bit_len, uint32 lit_arr_len,
        uint32 *distance_tree, uint8 *dist_tree_bit_len, uint32 dist_arr_len,
        uint32 *bytes_read)
{
    //1 MB data for the uncompressed block, you can pre allocte a giant memory
    // that you can pass in, the size of the memory would probaly be something like
    // bytes_per_pixel * width * height + height*filter_byte
    //each row has a filter byte(s) in the beginning that you have to account for
    //when you decompress

    uint8 *decompressed_data = sh_memalloc(1024*1024);
    uint32 data_index = 0;
    while(true) {
        uint32 decoded_value = sh_decode_huffman(bits, literal_tree, lit_code_bit_len, lit_arr_len);

        if(decoded_value == 256) break;
        if(decoded_value < 256) { //its a literal so just output it
            decompressed_data[data_index++] = decoded_value;
            continue;
        }

        if(decoded_value < 286 && decoded_value > 256) {
            uint32 base_index = decoded_value - 257;
            uint32 duplicate_length = base_lengths[base_index] + sh_png_read_bits(bits, base_length_extra_bit[base_index]);;

            uint32 distance_index = sh_decode_huffman(bits, distance_tree, dist_tree_bit_len, dist_arr_len);
            uint32 distance_length = dist_bases[distance_index] + sh_png_read_bits(bits, dist_extra_bits[distance_index]);

            uint32 back_pointer_index = data_index - distance_length;
            while(duplicate_length--) {
                decompressed_data[data_index++] = decompressed_data[back_pointer_index++];
            }

        }

    }

    *bytes_read = data_index;
    uint8 *fit_image = sh_memalloc(data_index);
    sh_memcpy(decompressed_data, fit_image, data_index);

    sh_memfree(decompressed_data);

    return fit_image;
}


uint8* sh_zlib_decompress(uint8 *zlib_data, uint32 *decompressed_size) {

    uint8 *decompressed_data = sh_memalloc(1024*1024*4); //4 MB free space
    uint32 data_read = 0;
    uint32 final;
    uint32 type;
    sh_png_bit_stream bit_stream = {zlib_data, 0, 0};
    sh_png_bit_stream *bits = &bit_stream;
    do {
        final = sh_png_read_bits(bits, 1);
        type = sh_png_read_bits(bits, 2);

        uint32 hlit = sh_png_read_bits(bits, 5) + 257;
        uint32 hdist = sh_png_read_bits(bits, 5) + 1;
        uint32 hclen = sh_png_read_bits(bits, 4) + 4;

        uint8 code_lengths_of_code_length_order[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        uint8 code_length_of_code_length[19]; //Longeure des 19 codes
        sh_memset(code_length_of_code_length, 0, 19);

        for(uint8 i = 0; i < hclen; ++i) {
            code_length_of_code_length[code_lengths_of_code_length_order[i]] = sh_png_read_bits(bits, 3);
        }

        uint32 *huffman_codes_of_tree_of_trees = sh_build_huffman_code(code_length_of_code_length, 19);
        uint8 *two_trees_code_bit_lengths = sh_memalloc(hlit + hdist);

        //because we have repetition, we won't necessarly have the exact bit lengths for each symbol if we just loop one increment at a time
        uint32 code_index = 0;
        while(code_index < (hdist+hlit)) {
            uint32 decoded_value = sh_decode_huffman(bits, huffman_codes_of_tree_of_trees, code_length_of_code_length, 19);
            if(decoded_value < 16) {
                two_trees_code_bit_lengths[code_index++] = decoded_value;
                continue;
            }

            uint32 repeat_count = 0;
            uint8 code_length_to_repeat = 0; 

            switch(decoded_value) {
                case 16:
                    repeat_count = sh_png_read_bits(bits, 2) + 3;// 3 - 6 repeat count
                    code_length_to_repeat = two_trees_code_bit_lengths[code_index - 1];
                    break;
                case 17:
                    repeat_count = sh_png_read_bits(bits, 3) + 3;// 3 - 10 repeat count
                    break;
                case 18:
                    repeat_count = sh_png_read_bits(bits, 7) + 11;// 3 - 10 repeat count
                    break;
            }

            sh_memset(two_trees_code_bit_lengths + code_index, code_length_to_repeat, repeat_count);
            code_index += repeat_count;
        }

        uint32 *literal_length_huff_tree = sh_build_huffman_code(two_trees_code_bit_lengths, hlit);
        uint32 *distance_huff_tree = sh_build_huffman_code(two_trees_code_bit_lengths + hlit, hdist);



        uint32 block_size = 0;
        uint8 *decompressed_block = sh_zlib_deflate_block(
                bits,
                literal_length_huff_tree, two_trees_code_bit_lengths, hlit,
                distance_huff_tree, two_trees_code_bit_lengths + hlit, hdist,
                &block_size);
        
        sh_memcpy(decompressed_block, decompressed_data + data_read, block_size);
        data_read += block_size;
        sh_memfree(decompressed_block);

    } while(!final);

    *decompressed_size = data_read;

    return decompressed_data;
}


uint8* sh_png_defilter(uint8 *decompressed_image, uint32 size, sh_png_chunk *ihdr) {
    uint32 x = sh_get_uint32be(ihdr->data);
    uint32 y = sh_get_uint32be(ihdr->data+4);
    uint8 bit_depth = *( ihdr->data + 4 + 4 );//count the bytes out
    uint8 byte_per_pixel = 1;//this is usually determined by checking color type, the picture I'm using is only greyscale, its only one byte per pixel

    uint8 *row = decompressed_image;
    uint32 stride = x*byte_per_pixel;

    uint8 *image = sh_memalloc(x*y*byte_per_pixel); //this is even smaller than the filter but just being safe
    uint8 *working = image;
    for(uint32 i = 0; i < y; ++i) {
        working = image + i*stride;
        uint8 filter = *row++;

        switch(filter) {
            case sh_no_filter: {
                for(uint32 j = 0; j < x; ++j) {
                    working[j] = row[j];
                }
            } break;

            case sh_sub_filter: {
                for(uint32 j = 0; j < x; ++j) {
                    uint8 a = 0;
                    if(j != 0) {
                        a = working[j-1];
                    }
                    uint8 value = row[j] + a;
                    working[j] = value;
                }
            } break;

            case sh_up_filter: {
                uint8 *prev_row = working - stride;
                for(uint32 j = 0; j < x; ++j) {
                    uint8 b = prev_row[j];
                    uint8 value = row[j] + b;
                    working[j] = value;
                }
           } break;

            case sh_avg_filter: {
                uint8 *prev_row = working - stride;
                for(uint32 j = 0; j < x; ++j) {
                    uint8 a = 0;
                    uint8 b = prev_row[j];
                    if(j) {
                        a = working[j - 1];
                    }

                    uint8 value = row[j] + ( (a + b) >> 1 );
                    working[j] = value;
                }
            } break;

            case sh_paeth_filter: {
                uint8 *prev_row = working - stride;
                for(uint32 j = 0; j < x; ++j) {
                    uint8 a = 0;
                    uint8 b = prev_row[j];
                    uint8 c = 0;
                    if(j) { 
                        a = working[j - 1];
                        c = prev_row[j - 1];
                    }

                    uint8 value = row[j] + sh_png_paeth_predict((int32) a, (int32) b, (int32) c);
                    working[j] = value;
                }
            } break;

        }

        row += stride;
    }

    return image;
}

int main(int argc, char **argv) {
    uint8 *mem = sh_read_file("sh_font_0.png");
    SKIP_BYTES(mem, 8); //skip signature, you can read it and check for stuff

    sh_png_chunk chunks[3];
    
    uint8 *for_reading = mem; //we don't want to lose the pointer of the file
    for(uint8 i = 0; i < 3; ++i) {
        chunks[i] = sh_png_read_chunk(for_reading);
        SKIP_BYTES(for_reading, 4 + 4 + chunks[i].data_length + 4); //Length bytes, type, actual data of the chunk, crc value 
    }
    //First chunk is IHDR, 2nd is one IDATA chunk, last one is IEND
    sh_zlib_block zlib_block = sh_read_zlib_block(chunks[1].data, chunks[1].data_length);

    uint32 decompressed_data_size = 0;
    uint8 *decompressed_png = sh_zlib_decompress(zlib_block.data, &decompressed_data_size);

    uint8 *image = sh_png_defilter(decompressed_png, decompressed_data_size, &chunks[0]);
    return 0;
}