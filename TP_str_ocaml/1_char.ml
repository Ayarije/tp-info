let rec ascii_to_id ascii =
  match ascii with
  | hd::tl when tl = [] -> [int_of_char hd]
  | hd::tl -> (int_of_char hd)::(ascii_to_id tl);;

let rec get_ascii_table max =
  match max with
  | 0 -> [char_of_int 0]
  | n -> (char_of_int n)::(get_ascii_table (n-1))

let ascii = get_ascii_table 127;;
let ascii_id = ascii_to_id ascii;;

let rec string_to_list str i =
  match str with
  |s when String.length s = i -> []
  |s -> (s.[i])::(string_to_list str (i+1));;

let rec list_to_string lst =
  match lst with
  |hd::tl when tl = [] -> String.make 1 hd
  |hd::tl -> (String.make 1 hd) ^ (list_to_string tl);;

let str = "Hello World >-<";;
let lst_str = string_to_list str 0;;
let str_str = list_to_string lst_str;;

let world = String.sub str 6 5;;
