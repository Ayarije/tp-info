type coup = Pierre | Feuille | Ciseaux;;

let rec winning_move c1 c2 =
  match c1, c2 with
  |Pierre, Ciseaux -> Pierre
  |Ciseaux, Feuille -> Ciseaux
  |Feuille, Pierre -> Feuille
  |c1, c2 when c1 = c2 -> c1
  |_, _ -> winning_move c2 c1;;

let win = winning_move Pierre Pierre;;

type var_coup = Pierre of int | Feuille of int | Ciseaux of int;;

let get_val c =
  match c with
  |Pierre n -> n
  |Feuille n -> n
  |Ciseaux n -> n;;

let is_same_type c1 c2 =
  match c1, c2 with
  |Pierre a, Pierre b -> true
  |Ciseaux a, Ciseaux b -> true
  |Feuille a, Feuille b -> true
  |_, _ -> false;;

let rec var_winning_move c1 c2 =
  let get_winner w l =
    if (get_val w) mod 2 = (get_val l) mod 2 then
      w
    else
      l
  in
  match c1, c2 with
  |Pierre n1, Ciseaux n2 -> get_winner (Pierre n1) (Ciseaux n2)
  |Ciseaux n1, Feuille n2 -> get_winner (Ciseaux n1) (Feuille n2)
  |Feuille n1, Pierre n2 -> get_winner (Feuille n1) (Pierre n2)
  |c1, c2 when is_same_type c1 c2 -> c1
  |_, _ -> var_winning_move c2 c1;;

let a = Pierre 20;;
let b = Feuille 21;;
let var_win = var_winning_move a b;;

type binary_tree = Leaf of int | Node of int * binary_tree * binary_tree;;

let rec sum_tree tree =
  match tree with
  |Leaf(n) -> n
  |Node(n, b1, b2) -> n + sum_tree b1 + sum_tree b2;;

let tree = Node(7, Node(2, Leaf(5), Leaf(6)), Leaf(4));;
let sum = sum_tree tree;;
