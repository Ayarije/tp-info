let print_file filepath =
  let filechannel = open_in filepath in
  let rec get_lines channel =
    try
      let l = input_line channel in
      l ^ "\n" ^ (get_lines channel)
    with
    | End_of_file -> ""
  in
  let lines = get_lines filechannel in
  let _ = close_in filechannel in
  print_string lines;;

let _ = print_file "lorem.txt";;

let paste_int filepath lst =
  let chan = open_out filepath in
  let rec print_into chan lst =
    match lst with
    | hd::tl when tl = [] -> Printf.fprintf chan "%d\n" hd
    | hd::tl -> let _ = Printf.fprintf chan "%d\n" hd in print_into chan tl
  in
  let _ = print_into chan lst in
  close_out chan;;

let _ = paste_int "int.txt" [1;2;3;4;5;6;7;8;9];;
