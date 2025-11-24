(* Type pour les expressions rationnelles *)
type regex =
  | Vide
  | Epsilon
  | Lettre of char
  | Somme of regex * regex
  | Produit of regex * regex
  | Etoile of regex

type taille =
  | Non_borne
  | Aucun_mot
  | Entier of int;;

exception Est_vide;;

(** Entrée : une chaîne représentant une expression rationnelle. Epsilon est représenté par &, Vide par le caractère #, les espaces sont ignorées et les règles de priorité sont celles usuelles (etoile > produit > somme).
Sortie : un objet de type regex correspondant à la chaîne en entrée *)
let parser string =
  let open Printf in
  let s = Stream.of_string string in
  let rec peek () =
    match Stream.peek s with
    | Some ' ' -> Stream.junk s; peek ()
    | Some c -> Some c
    | None -> None in
  let eat x =
    match peek () with
    | Some y when y = x -> Stream.junk s; ()
    | Some y -> failwith (sprintf "expected %c, got %c" x y)
    | None -> failwith "incomplete" in
  let rec regex () =
    let t = term () in
    match peek () with
    | Some '|' -> eat '|'; Somme (t, regex ())
    | _ -> t
  and term () =
    let f = factor () in
    match peek () with
    | None | Some ')' | Some '|' -> f
    | _ -> Produit (f, term ())
 and factor () =
    let rec aux acc =
      match peek () with
      | Some '*' -> eat '*'; aux (Etoile acc)
      | _ -> acc in
    aux (base ())
  and base () =
    match peek () with
    | Some '(' -> eat '('; let r = regex () in eat ')'; r
    | Some '&' -> eat '&'; Epsilon
    | Some '#' -> eat '#'; Vide
    | Some (')' | '|' | '*' as c) -> failwith (sprintf "unexpected '%c'" c)
    | Some c -> eat c; Lettre c
    | None -> failwith "unexpected end of string" in
  let r = regex () in
  try Stream.empty s; r
  with _ -> failwith "trailing ')' ?"

(* Exemples de parsage *)
let e = parser "ab**"
let e' = parser "(b|&)(ba|a)&"
let e_vide = parser "(a|b)&(c)#"

let rec est_vide re =
  match re with
  | Vide -> true
  | Epsilon -> false
  | Lettre c -> false
  | Somme(re1, re2) -> est_vide re1 || est_vide re2
  | Produit(re1, re2) -> est_vide re1 || est_vide re2
  | Etoile re -> est_vide re;;

let rec donner_mot re =
  if est_vide re then
    raise Est_vide
  else match re with
    | Lettre c -> String.make 1 c
    | Epsilon -> "a"
    | Etoile re -> ""
    | Somme(re1, re2) -> donner_mot re1
    | Produit(re1, re2) -> donner_mot re1 ^ donner_mot re2;;

let rec longueur_max re =
  let somme_regex e1 e2 =
    let l1 = longueur_max e1 in
    let l2 = longueur_max e2 in
    match l1, l2 with
    | Non_borne, Entier(_) -> Non_borne
    | Entier(_), Non_borne -> Non_borne
    | Entier(l1), Entier(l2) -> Entier(max l1 l2)
  in
  let produit_regex e1 e2 =
    let l1 = longueur_max e1 in
    let l2 = longueur_max e2 in
    match l1, l2 with
    | Non_borne, Entier(_) -> Non_borne
    | Entier(_), Non_borne -> Non_borne
    | Entier(l1), Entier(l2) -> Entier(l1 + l2)
  in
  if est_vide re then
    Aucun_mot
  else match re with
    | Lettre c -> Entier(1)
    | Epsilon -> Entier(1)
    | Etoile(e) -> Non_borne
    | Somme(e1, e2) -> somme_regex e1 e2
    | Produit(e1, e2) -> produit_regex e1 e2;;
       

let e_w = donner_mot e;;
let e'_w = donner_mot e';;
let e_vide_w = donner_mot e_vide;;

let e_len = longueur_max e;;
let e'_len = longueur_max e';;
let e_vide_len = longueur_max e_vide;;

