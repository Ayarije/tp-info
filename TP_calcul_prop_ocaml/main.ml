exception NotExhaustiveValuation;;
type formula =
  | Var of int
  | Top
  | Bot
  | Not of formula
  | Or of formula * formula
  | And of formula * formula
  | Imply of formula * formula;;

let f1 = Imply(Var(4), Imply(Imply(Var(3), Var(2)), Var(1)));;
let f2 = Or(And(Var(1), Imply(Var(2), Var(1))), Bot);;

let rec formula_height formule =
  match formule with
  | Var(id)       -> 0
  | Top           -> 0
  | Bot           -> 0
  | Not(f)        -> 1 + (formula_height f)
  | Or(f1, f2)    -> 1 + (max (formula_height f1) (formula_height f2))
  | And(f1, f2)   -> 1 + (max (formula_height f1) (formula_height f2))
  | Imply(f1, f2) -> 1 + (max (formula_height f1) (formula_height f2));;

let f1_h = formula_height f1;;
let f2_h = formula_height f2;;

let rec formula_size formule =
  match formule with
  | Var(id)       -> 1
  | Top           -> 1
  | Bot           -> 1
  | Not(f)        -> 1 + (formula_size f)
  | Or(f1, f2)    -> 1 + (formula_size f1) + (formula_size f2)
  | And(f1, f2)   -> 1 + (formula_size f1) + (formula_size f2)
  | Imply(f1, f2) -> 1 + (formula_size f1) + (formula_size f2);;

let f1_size = formula_size f1;;
let f2_size = formula_size f2;;

let rec get_value id valuation =
  match valuation with
  | [] -> raise NotExhaustiveValuation
  | (i, v)::tail when i = id -> v
  | hd::tail -> get_value id tail;;

let rec evaluate valuation formule =
  match formule with
  | Var(id)       -> get_value id valuation
  | Top           -> true
  | Bot           -> false
  | Not(f)        -> not (evaluate valuation f)
  | Or(f1, f2)    -> (evaluate valuation f1) || (evaluate valuation f2)
  | And(f1, f2)   -> (evaluate valuation f1) && (evaluate valuation f2)
  | Imply(f1, f2) -> not (evaluate valuation f1) || (evaluate valuation f2);;

let vals = [(1, true); (2, false); (3, false)];;

let f1_v = evaluate vals f1;;
let f2_v = evaluate vals f2;;

let rec uni_concat lst1 lst2 =
  let add e lst =
    if List.mem e lst then lst
    else                   e::lst
  in
  match lst1, lst2 with
  | [], []    -> []
  | v::tl, _  -> add v (uni_concat tl lst2)
  | [], v::tl -> add v (uni_concat lst1 tl)

let rec variable_list formula =
  match formula with
  | Var(id)       -> [id]
  | Top           -> []
  | Bot           -> []
  | Not(f)        -> variable_list f
  | Or(f1, f2)    -> uni_concat (variable_list f1) (variable_list f2)
  | And(f1, f2)   -> uni_concat (variable_list f1) (variable_list f2)
  | Imply(f1, f2) -> uni_concat (variable_list f1) (variable_list f2);;

let f1_var_lst = variable_list f1;;
let f2_var_lst = variable_list f2;;

let rec fast_exp a n =
  match a, n with
  | a, 0 -> 1
  | a, k when k mod 2 = 0 -> fast_exp (a*a) (k/2)
  | a, k -> a*fast_exp a (k-1);;

let rec generate_all_valuations var_lst incr =
  let rec bin_of_int n len =
    match n, len with
    | _, 0 -> []
    | n, l -> (if n mod 2 = 0 then false else true)::(bin_of_int (n/2) (len - 1))
  in
  let rec gen_val var_lst incr =
    match var_lst, incr with
    | [], []                  -> []
    | var::var_tl, v::incr_tl -> (var, v)::(gen_val var_tl incr_tl)
  in
  let len = List.length var_lst in
  let threshold = fast_exp 2 len in
  match incr with
  | i when i >= threshold -> []
  | i -> (gen_val var_lst (bin_of_int i len))::(generate_all_valuations var_lst (i + 1));;

let f1_vals = generate_all_valuation f1_var_lst 0;;
let f2_vals = generate_all_valuation f2_var_lst 0;;

let is_satisfiable formula =
  let f_vals = generate_all_valuations (variable_list formula) 0 in
  let rec go_through_vals vals =
    match vals with
    | [] -> false
    | valuation::tail ->
       if evaluate valuation formula then
         true
       else
         go_through_vals tail
  in
  go_through_vals f_vals;;

let is_f1_sat = is_satisfiable f1;;
let is_f2_sat = is_satisfiable f2;;
let is_Top_sat = is_satisfiable Top;;
let is_Bot_sat = is_satisfiable Bot;;

let is_contradiction formula = not (is_satisfiable formula);;

let is_f1_ant = is_contradiction f1;;
let is_f2_ant = is_contradiction f2;;
let is_Top_ant = is_contradiction Top;;
let is_Bot_ant = is_contradiction Bot;;

let rec subformulas formula =
  match formula with
  | Var(id) -> Var(id)
  | Top -> Top
  | Bot -> Bot
