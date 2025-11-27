let rec string_to_list s i =
  match i with
  | i when i = String.length s -> []
  | i -> s.[i]::(string_to_list s (i + 1));;

let rec list_to_string lst =
  match lst with
  | hd::tl when tl = [] -> String.make 1 hd
  | hd::tl ->  (String.make 1 hd) ^ (list_to_string tl);;

let rec double_char str =
  let len = String.length str in
  match str with
  | s when len = 0 -> ""
  | s -> let c = String.make 1 s.[0] in
         c ^ c ^ (double_char (String.sub s 1 (len - 1)));;

let d_str = double_char "Hello World !";;

let rec count_char c t =
  let len = String.length t in
  match t with
  | t when len = 0 -> 0
  | t when t.[0] = c -> 1 + count_char c (String.sub t 1 (len-1))
  | t -> count_char c (String.sub t 1 (len-1));;

let how_many_l = count_char 'l' "Hello World !";;

let rec suppr_space str =
  let len = String.length str in
  match str with
  | s when len = 0 -> ""
  | s when s.[0] = ' ' -> "" ^ (suppr_space (String.sub s 1 (len-1)))
  | s -> let c = String.make 1 s.[0] in
         c ^ (suppr_space (String.sub s 1 (len-1)));;

let hw_without_space = suppr_space "Hello World !";;



let rec caesar_shift s k =
  let shift l k =
    let r = l + k in
    match r with
    | r when r > 122 -> r - 123 + 97
    | r when r < 97 -> r + 26
    | r -> r
  in
  let rec lst_shift ls k =
    match ls with
    | hd::tl when tl = [] -> [char_of_int (shift (int_of_char hd) k)]
    | hd::tl -> (char_of_int (shift (int_of_char hd) k))::(lst_shift tl k)
  in
  list_to_string (lst_shift (string_to_list s 0) k);;

let shifted_5 = caesar_shift "abcdefghijklmnopqrstuvwxyz" 5;;

let unshifted = caesar_shift shifted_5 (-5);;

let copy_file old_path new_path =
  let old_chan = open_in old_path in
  let new_chan = open_out old_path in
  let rec cp old_c new_c =
    try
      let l = input_line old_c in
      let _ = output_string new_c (l ^ "\n") in
      cp old_c new_c
    with
    | End_of_file -> true
  in
  let _ = cp old_chan new_chan in
  let _ = close_in old_chan in
  close_out new_chan;;

let _ = copy_file "lorem.txt" "lorem_bis.txt"
