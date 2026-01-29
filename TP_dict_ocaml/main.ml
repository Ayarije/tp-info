type ('a, 'b) hash_table = {data: ('a * 'b) list array; hash_function: 'a -> int};;

let init_hash_table n hash_f = {
    data = Array.make n [];
    hash_function = hash_f
  };;

exception KeyNotFound

let rec _find_value key lst =
  match lst with
  | [] -> raise KeyNotFound
  | (k, v)::tl ->
     if k = key then
       v
     else
       _find_value key tl;;

let rec _removed key lst =
  match lst with
  | [] -> raise KeyNotFound
  | (k, v)::tl ->
     if k = key then
       tl
     else
       (k, v)::(_removed key tl);;

let member key hash_table =
  let index = hash_table.hash_function key in
  try _find_value key hash_table.data.(index); true
  with KeyNotFound -> false

let get_value key hash_table =
  let index = hash_table.hash_function key in
  _find_value key hash_table.data.(index);;

let add_value key value hash_table =
  let index = hash_table.hash_function key in
  if not (member key hash_table) then
    hash_table.data.(index) <- (key, value)::hash_table.data.(index)
  else
    failwith "KeyAlreadyExists";;

let remove_element key hash_table =
  let index = hash_table.hash_function key in
  hash_table.data.(index) <- _removed key hash_table.data.(index);;

let items hash_table =
  let lst = ref [] in
  for i = 0 to Array.length hash_table.data - 1 do
    lst := hash_table.data.(i) @ !lst
  done;
  !lst;;

let simple_hash_fn size key = ((key mod size) + size) mod size;;

let simple_init size hash_function = {
    data = Array.make size [];
    hash_function = hash_function size
  };;

let table = simple_init 21 simple_hash_fn;;
for i = 0 to 50 do
  add_value i (string_of_int i) table
done;;
let t = table;;
print_string (get_value 5 table);;
print_string (get_value 8 table);;
print_string (get_value 2 table);;
remove_element 8 table;;
let t = table;;
let item = items table;;

let simple_string_hash_fn size str =
  let sum = ref 0 in
  for i = 0 to String.length str - 1 do
    let ascii_code = int_of_char str.[i] in
    sum := !sum + ascii_code
  done;
  !sum mod size;;

let table = init_hash_table 10 (string_hash_fn 10);;

add_value "eight" 8 table;;
add_value "nine" 9 table;;
add_value "ten" 10 table;;
add_value "fifteen" 15 table;;
add_value "fifty" 50 table;;
add_value "one" 1 table;;
add_value "three" 3 table;;
add_value "six" 6 table;;

let t = table;;

let advanced_string_hash_fn p size str =
  let rec _pow a p = function
  | 0 -> 1
  | 1 -> a
  | n -> 
    let b = _pow a p (n / 2) in
    b * b * (if n mod 2 = 0 then 1 else a) mod p
  in
  let sum = ref 0 in
  for i = 0 to String.length str - 1 do
    let ascii_code = int_of_char str.[i] in
    sum := (!sum + ascii_code * (_pow 128 p i)) mod p
  done;
  !sum mod size;;

let table = init_hash_table 256 (advanced_string_hash_fn 1114567 256);;

for i = 0 to 100 do
  add_value (string_of_int i) i table
done;;
  
let t = table;;
