/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 04:50:42 by scambier          #+#    #+#             */
/*   Updated: 2024/06/02 22:45:38 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "structs.h"
#include "ftpng.h"

t_uint8	find_max(t_uint8 *array, t_uint32 length)
{
	t_uint8	out;

	out = 0;
	while (length-- > 0)
		if (array[length] > out)
			out = array[length];
	return (out);
}

void	count_lengths(t_uint32 *out, t_uint8 *lengths_array, t_uint32 length)
{
	for(t_uint32 i = 0; i < length; ++i) {
        out[lengths_array[i]]++;
    }
}

// void sh_first_code_for_bitlen(uint32 *first_codes, uint32 *code_count, uint32 max_bit_length) {
//     uint32 code = 0;
//     for(uint32 i = 1; i <= max_bit_length; ++i) {
//         code = ( code + code_count[i-1]) << 1; 

//         if(code_count[i] > 0) {
//             first_codes[i] = code;
//         }
//     }
// }

void	assign_firsts_codes(t_uint32 *out, t_uint32 *code_counts, t_uint32 length)
{
	t_uint32	code;
	int			k;

	code = 0;
	k = -1;
	while (++k < length)
	{
		code = (code + code_counts[k]) << 1;
		if (code_counts[k + 1] > 0)
			out[k + 1] = code;
	}
}

// void sh_assign_huffman_code(uint32 *assigned_codes, uint32 *first_codes, uint8 *code_bit_lengths, uint32 len_assign_code) {
//     for(uint32 i = 0; i < len_assign_code; ++i) {
//         if(code_bit_lengths[i]) {
//             assigned_codes[i] = first_codes[code_bit_lengths[i]]++;
//         }
//     }
// }

// uint32* sh_build_huffman_code(uint8 *code_bit_lengths, uint32 len_code_bit_lengths) {
//     uint32 max_bit_length = sh_get_maximum_bit_length(code_bit_lengths, len_code_bit_lengths);

//     uint32 *code_counts = (uint32 *)sh_memalloc(sizeof(uint32)*( max_bit_length + 1 ));
//     uint32 *first_codes = (uint32 *)sh_memalloc(sizeof(uint32)*(max_bit_length + 1));
//     uint32 *assigned_codes = (uint32 *)sh_memalloc(sizeof(uint32)*(len_code_bit_lengths));//we have to assign code to every element in the alphabet, even if we have to assign zero


//     sh_get_bit_length_count(code_counts,  code_bit_lengths, len_code_bit_lengths);
//     code_counts[0] = 0; //in the real world, when a code of the alphabet has zero bit length, it means it doesn't occur in the data thus we have to reset the count for the zero bit length codes to 0.

//     sh_first_code_for_bitlen(first_codes, code_counts, max_bit_length);
//     sh_assign_huffman_code(assigned_codes, first_codes, code_bit_lengths, len_code_bit_lengths);

//     return assigned_codes;
// }

void	assign_remaining_codes(t_uint32 *out, t_uint32 *first_codes, t_uint8 *code_lengths, t_uint32 length)
{
	int	k;

	k = -1;
	while (++k < length)
		if (code_lengths[k])
			out[k] = first_codes[code_lengths[k]]++;
}

// uint32 sh_decode_huffman(sh_png_bit_stream *bits, uint32 *assigned_codes, uint8 *code_bit_lengths, uint32 assigned_code_length) {
//     for(uint32 i = 0; i < assigned_code_length; ++i) {
//         if(code_bit_lengths[i] == 0) continue;
//         uint32 code = sh_peak_bits_reverse(bits, code_bit_lengths[i]);
//         if(assigned_codes[i] == code) {
//             bits->bit_buffer >>= code_bit_lengths[i];
//             bits->bits_remaining -= code_bit_lengths[i];
//             return i;
//         }
//     }

//     return 0;
// }

t_uint32	decode_huffman(t_bit_stream *stream, t_uint32 *assigned_codes, t_uint8 *lengths, t_uint32 arrays_length)
{
	t_uint32	peek;
	int			k;
	
	k = -1;
	while (++k < arrays_length)
	{
		if (lengths[k] == 0)
			continue ;
		peek = peek_bits_reversed(stream, lengths[k]);
		if (assigned_codes[k] == peek) {
			stream->bit_buffer >>= lengths[k];
			stream->bits_left -= lengths[k];
			return (k);
		}
	}
	return (0);
}

// uint32* sh_build_huffman_code(uint8 *code_bit_lengths, uint32 len_code_bit_lengths) {
//     uint32 max_bit_length = sh_get_maximum_bit_length(code_bit_lengths, len_code_bit_lengths);

//     uint32 *code_counts = (uint32 *)malloc(sizeof(uint32)*( max_bit_length + 1 ));
//     uint32 *first_codes = (uint32 *)malloc(sizeof(uint32)*(max_bit_length + 1));
//     uint32 *assigned_codes = (uint32 *)malloc(sizeof(uint32)*(len_code_bit_lengths));//we have to assign code to every element in the alphabet, even if we have to assign zero


//     sh_get_bit_length_count(code_counts,  code_bit_lengths, len_code_bit_lengths);
//     code_counts[0] = 0; //in the real world, when a code of the alphabet has zero bit length, it means it doesn't occur in the data thus we have to reset the count for the zero bit length codes to 0.

//     sh_first_code_for_bitlen(first_codes, code_counts, max_bit_length);
//     sh_assign_huffman_code(assigned_codes, first_codes, code_bit_lengths, len_code_bit_lengths);

//     return assigned_codes;
// }

t_uint32	*build_huffman_codes(t_uint8 *code_lengths, t_uint32 length)
{
	t_uint32	max_code_length = find_max(code_lengths, length);
	t_uint32	*counts = ft_calloc(max_code_length + 1, sizeof(t_uint32));
	t_uint32	*firsts_codes = ft_calloc(max_code_length + 1, sizeof(t_uint32));
	t_uint32	*assigned_codes = ft_calloc(length, sizeof(t_uint32));

	count_lengths(counts, code_lengths, length);
	counts[0] = 0;

	assign_firsts_codes(firsts_codes, counts, max_code_length);
	free(counts);
	
	assign_remaining_codes(assigned_codes, firsts_codes, code_lengths, length);
	free(firsts_codes);
		
	return (assigned_codes);
}

t_uint8 base_length_extra_bit[] = {
    0, 0, 0, 0, 0, 0, 0, 0, //257 - 264
    1, 1, 1, 1, //265 - 268
    2, 2, 2, 2, //269 - 273 
    3, 3, 3, 3, //274 - 276
    4, 4, 4, 4, //278 - 280
    5, 5, 5, 5, //281 - 284
    0           //285
};

t_uint32 base_lengths[] = {
    3, 4, 5, 6, 7, 8, 9, 10, //257 - 264
    11, 13, 15, 17,          //265 - 268
    19, 23, 27, 31,          //269 - 273 
    35, 43, 51, 59,          //274 - 276
    67, 83, 99, 115,         //278 - 280
    131, 163, 195, 227,      //281 - 284
    258                      //285
};

t_uint32 dist_bases[] = {
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

t_uint32 dist_extra_bits[] = {
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

void	decompress_zblock(t_bit_stream *stream)
{
	t_uint32 is_last;
	t_uint32 type;
	t_uint32 hlit; //Huffman literals (len)
	t_uint32 hdist; //Huffman distances (len)
	t_uint32 hclen; //Huffman codes len (len)

	//Lecture du header du block
	is_last = read_bits(stream, 1);
	ft_printf("\nIs last: %x\n", is_last);
	type = read_bits(stream, 2);
	ft_printf("Block type: %x\n", type);
	if (type == 0)
		ft_printf("No compression\n");
	else if (type == 1)
		ft_printf("Fixed huffman code\n");
	else if (type == 2)
		ft_printf("Dynamic huffman code\n");
	else if (type == 3)
		ft_printf("Error\n");
	if (type != 2)
		return ;
	hlit = read_bits(stream, 5) + 257;
	ft_printf("hlit: %d - 257\n", hlit);
	hdist = read_bits(stream, 5) + 1;
	ft_printf("hdist: %d - 1\n", hdist);
	hclen = read_bits(stream, 4) + 4;
	ft_printf("hclen: %d - 4\n", hclen);

	//Lecture des longueur des codes
	t_uint8 reorder_indexes[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
	t_uint8 code_lengths[19]; //Longeure des 19 codes
	ft_memset(code_lengths, 0, 19);
	for(t_uint8 i = 0; i < hclen; ++i)
		code_lengths[reorder_indexes[i]] = read_bits(stream, 3);

	//Assignation des codes
	t_uint32	*assigned_codes = build_huffman_codes(code_lengths, 19);

	//Lecture des deux arbres (literal & repetitions)
	ft_printf("Reading global tree\n");
	t_uint8	*both_trees_codes = ft_calloc(hlit + hdist, sizeof(t_uint8));
	t_uint32 code_index = 0;
	while(code_index < (hdist+hlit))
	{
		t_uint32	decoded_value = decode_huffman(stream, assigned_codes, code_lengths, 19);
		if (decoded_value < 16)
		{
			both_trees_codes[code_index++] = decoded_value;
			continue ;
		}
		
		t_uint32 repeat_count = 0;
		t_uint8 repeat_value = 0;

		if (decoded_value == 16)
		{
			repeat_count = read_bits(stream, 2) + 3;
			repeat_value = both_trees_codes[code_index - 1];
		}
		else if (decoded_value = 17)
		{
			repeat_count = read_bits(stream, 3) + 3;
		}
		else if (decoded_value = 18)
		{
			repeat_count = read_bits(stream, 7) + 11;
		}
		ft_memset(both_trees_codes + code_index, repeat_value, repeat_count);
		code_index += repeat_count;
	}
	free(assigned_codes);

	//ft_pmem(both_trees_codes, hlit + hdist);

	//Construction des deux arbres de huffman
	ft_printf("Assigning codes\n");
	t_uint32 *literal_tree = build_huffman_codes(both_trees_codes, hlit);
	t_uint32 *distance_tree = build_huffman_codes(both_trees_codes + hlit, hdist);

	//Decompression de la data, en utilisant des deux arbres pour LZ77
	ft_printf("Reading LZ77\n");
	t_uint8	buffer[1024 * 1024];
	t_uint32	index;
	t_uint32	value;

	index = 0;
	while (1)
	{
		value = decode_huffman(stream, literal_tree, both_trees_codes, hlit);
		// ft_printf("Decoded : %u\n", value);
		if (value == 256)
			break ;
		else if (value < 256)
			buffer[index++] = value;
		else if (value < 286)
		{
			t_uint32	base_index = value - 257;
			t_uint32	duplicate_length = base_lengths[base_index] + read_bits(stream, base_length_extra_bit[base_index]);;

			t_uint32	distance_index = decode_huffman(stream, distance_tree, both_trees_codes + hlit, hdist);
            t_uint32	distance_length = dist_bases[distance_index] + read_bits(stream, dist_extra_bits[distance_index]);
			
			t_uint32 back_pointer_index = index - distance_length;
            while(duplicate_length--) {
                buffer[index++] = buffer[back_pointer_index++];
            }
		}
		else
			ft_printf("Macarena >= 286\n");
	}

	//Copy
	ft_printf("Copying result\n");
	t_uint8	*out = ft_calloc(index, sizeof(t_uint8));
	ft_memcpy(out, buffer, index);

	ft_pmem(out, index);
	ft_printf("Read %u bytes\n", index);

	// ft_hexdump(out, index, 4);

	//Cleanup
	free(distance_tree);
	free(literal_tree);
}

/*
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
*/

int	inflate(t_uint8 *dst, t_uint32 dst_len, t_uint8 *src, t_uint32 src_len)
{
	t_uint8			cmf;
	t_uint8			flags;
	t_bit_stream	stream;
	t_uint16		check_value;
	t_uint8			type;

	cmf = src[0];
	flags = src[1];
	stream.data = src + 2;
	stream.bit_buffer = 0;
	stream.bits_left = 0;
	check_value = src[src_len - 2] << 1 | src[src_len - 1];

	
	decompress_zblock(&stream);

	
	return (1);
}

/*
hlit : number of literals/length codes (all literals and the stop code)
hdist : number of distance codes
hclen :
*/