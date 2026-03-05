type ('a, 'b) dict = Empty | Node of ('a * 'b) * ('a, 'b) dict * ('a, 'b) dict;;

let rec infix_traversal d f =
  match d with
  | Empty -> ()
  | Node(data, child1, child2) ->
     let k, v = data in
     infix_traversal child1 f;
     f k v;
     infix_traversal child2 f;;



let rec get_value d key =
  match d with
  | Empty -> failwith "Can't find the key"
  | Node(data, left, right) ->
     let k, v = data in
     if k = key then
       v
     else if key < k then
       get_value right key
     else
       get_value left key;;

let a = get_value d "texte";;

let make_dict () = Empty;;

let rec insert d key value =
  match d with
  | Empty -> Node((key, value), Empty, Empty)
  | Node(data, left, right) ->
     let k, v = data in
     if k = key then
       failwith "Key already exists"
     else if key < k then
       Node(data, insert left key value, right)
     else
       Node(data, left, insert right key value);;

let d = Empty;;
let d = insert d "mot" 5;;
let d = insert d "lettre" 12;;
let d = insert d "phrase" 7;;
let d = insert d "texte" 1;;
infix_traversal d (fun k -> (fun v -> (Printf.printf "%s: %d\n" k v)));;

let new_d = insert d "caca" 6;;
infix_traversal new_d (fun k -> (fun v -> (Printf.printf "%s: %d\n" k v)));;

let rec remove d key =
  let rec remove_root_to_left d =
    match d with
    | Empty -> failwith "Can't remove in an empty tree"
    | Node(data, left, Empty) -> (data, left)
    | Node(data, left, right) ->
       let rep_data, new_left = remove_root_to_left left in
       (rep_data, Node(data, new_left, right))
  in
  match d with
  | Empty -> failwith "Key is not in the dict"
  | Node(data, left, right) ->
     let k, v = data in
     if k = key then
       let new_data, new_left = remove_root_to_left left in
       Node(new_data, new_left, right)
     else if key < k then
       let new_left = remove left key in
       Node(data, new_left, right)
     else
       let new_right = remove right key in
       Node(data, left, new_right);;

let d_peigne_droite = Empty;;
let d_peigne_droite = insert d_peigne_droite 1 0;;
let d_peigne_droite = insert d_peigne_droite 3 0;;
let d_peigne_droite = insert d_peigne_droite 4 0;;
let d_peigne_droite = insert d_peigne_droite 5 0;;
let d_peigne_droite = insert d_peigne_droite 2 0;;

let rem_d = remove d_peigne_droite 3;;
           
let rec dict_to_string d sp_key sp_val =
  let d_to_s d = dict_to_string d sp_key sp_val in
  match d with
  | Empty -> ""
  | Node((key, value), left, right) ->
     (d_to_s left) @ (d_to_s right) @ ";" @ (sp_key key) @ ":" @ (sp_val value)

let sp_key k = k;;
let sp_val v = Printf.sprintf "%d" v;;
