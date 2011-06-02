(program 

// -------------------------------------------------
// Tests if two lists are equal
// -------------------------------------------------
(= list_equal (lambda (x y)
	(if (|| (is_nil x) (is_nil y))
		(&& (is_nil x) (is_nil y))
		(&& (==			(car x) (car y))
			(list_equal (cdr x) (cdr y))
		)
	)
))

// -------------------------------------------------
// Appends two lists to each other
// -------------------------------------------------
(= append (lambda (m n)
	(if (is_nil m)
		n
		(program 
			(cons (car m) (append (cdr m) n))
		)
	)
))

// -------------------------------------------------
// Filters a list with a binary predicate and a given value
// -------------------------------------------------
(= filter (lambda (q bin_pred val)
	(if (is_nil q)
		(quote ())
		(if (bin_pred (car q) val)
			(cons (car q) (filter (cdr q) bin_pred val))
							(filter (cdr q) bin_pred val)
		)
	)
))

// -------------------------------------------------
// Quicksorts a list
// -------------------------------------------------
(= quick_sort (lambda (list)
	(if (is_nil list)
		(quote ())
		(append
			(quick_sort		(filter list (lambda (x y) (<  x y)) (car list)))
			(append
							(filter list (lambda (x y) (== x y)) (car list))
				(quick_sort (filter list (lambda (x y) (>  x y)) (car list)))
			)
		)
	)
))

// -------------------------------------------------
// Test the quick sort
// -------------------------------------------------
(list_equal 
	(quick_sort (quote (4 6 7 8 9 3 4 2 5 5 6 7 8 9 2 3 5 3 3 1 3 0))) 
				(quote (0 1 2 2 3 3 3 3 3 4 4 5 5 5 6 6 7 7 8 8 9 9)
	)
)

)
