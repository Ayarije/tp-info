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

(* Q9. O(n) avec n la taille de la liste à l'indice donnée par la fonction de hachage *)
(* Q10. Oui car on n'aurait qu'un unique couple par indice dans le tableau *)
(* Q11. Une clef négative renvoie un mod négatif -> pas bien *)

(* Q12. *)
let iter hash_table f =
  let rec apply lst f =
    match lst with
    | [] -> []
    | (k, v)::tl -> (f k v)::(apply tl f)
  in
  for i = 0 to Array.length hash_table.data - 1 do
    hash_table.data.(i) <- apply hash_table.data.(i) f
  done;;
    
(* Q13. tous les anagrammes ont la même images par la fonction du 5. *)
(* Q14. la table du dictionnaire sera vide au delà de celui-ci *)

(* Q15. *)
let symmetry t =
  let add_key t key value =
    Hashtbl.replace t (-key) value
  in
  Hashtbl.iter (add_key t) t;;

let table = Hashtbl.create 56;;

for i = -10 to 20 do
  Hashtbl.add table i i
done;;
Hashtbl.iter (fun k v -> Printf.printf "%d: %d\n" k v) table;;
symmetry table;;
Hashtbl.iter (fun k v -> Printf.printf "%d: %d\n" k v) table;;

