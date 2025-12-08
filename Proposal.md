 File compression using Huffman encoding and few others

|Id|Name|
|-|-|
|k24-2540|Muhammad Amaan Jilani|
|K24-2515|Ahmed Raza|

## Introduction
In an age where data storage and transfer efficiency are crucial, file compression techniques play a key role in minimizing file sizes
without losing information. This project focuses on implementing File Compression using Huffman Encoding — a lossless compression 
method that assigns variable-length binary codes to characters based on their frequency of occurrence. More frequent characters 
get shorter codes, resulting in reduced overall file size. Additionally, the project will explore and compare other compression
techniques to evaluate their effectiveness in different scenarios.
## Description
Read an input file (text or binary).

Analyze the frequency of each character or symbol in the file.

Build a Huffman Tree based on symbol frequencies.

Generate unique binary codes for each symbol (shorter for frequent ones).

Compress the file by replacing symbols with their corresponding Huffman codes.

Store the encoding table alongside the compressed file for decompression.

Implement the decompression process to reconstruct the original file.

Optionally, compare Huffman encoding with other methods (e.g., Run-Length Encoding, LZW).


## Data Structures that will be used
|        Data Structure          |        Purpose / Where Used         |                                How It Will Be Used                                                 |
| ------------------------------ | ----------------------------------- | -------------------------------------------------------------------------------------------------- |
| Array / Map                    | Storing character frequencies       | Count how many times each symbol appears in the input file.                                        |
| Max-Heap / Priority Queue      | Building the Huffman Tree           | Select two largest frequency nodes repeatedly to build the optimal Huffman tree.                  |
| Binary Tree (Huffman Tree)     | Encoding structure                  | Nodes represent characters and their frequencies; left/right traversal determines code generation. |
| Hash Table / Dictionary        | Mapping characters to Huffman codes | Store generated binary codes for quick lookup during encoding and decoding.                        |
| String / Bit Array             | Compressed data representation      | Store and manipulate binary sequences of encoded data.                                             |
| File I/O Streams               | Reading and writing files           | Handle input (original file) and output (compressed and decompressed files).                       |



