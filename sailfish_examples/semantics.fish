/*
	List Definition type checks:
		- must be type that exists
		- expression must match the declared type
	
	Dictionary Definition type checks:
		- must be types (key and value) that exist
		- expression must match the declared types
	
	UDT
		- method outputs and inputs must be types that exist
		- not using a reserved name as the key
		- attributes must be unique names
		- attribute types must exist

	return
		- must have type that matches existed
		
	assignment
		- types must exist
	
	functioncall
		- function must exist
		- params must match expected
		- return must match
	
	list literal
		- all elements must be expected type and same type
	
	dictionary literal
		- all elements must be expected type and same type

	grouping
		- must ultimately evaluate to boolean (?)

	attribute access
		- must be on a udt that exists
		- attribute must also exist

	method access
		- must be on a udt that exists
		- method must exist
	
	user defined type
		- all types must match for assignments
		- all attributes must match and exist
		- all types must exist
	
	Integer/String/Float
		- watch for overflow

	list def
		- not a resereved keyword

	dict def
		- not a reserved keyword

	var decl
		- type exists
		- matching types
		- not a reserved keyword
*/

// -------       function      -------- //
/*
fun good
<- void
-> void
{}

fun inputDoesNotExist
<- nonExistentType foo
-> void
{}

fun outPutDoesNotExist
<- void
-> nonExistentType
{}

*/

// --------       declerations         --------
dec flt a = 10 // good
dec fat b = 10 // error type does not exist
dec flt flt = 10.0 // error name is a primitive
dec flt dec = 10.0 // error name is a keyword
dec flt foo = 10 // error expression type and assignment type are different

/*
start {
    // +
    f = 10.0 + 10.0 // ok
    f = 1 + 2 // ok
    f = false + true // error
    f = 10 + 10.0 // error
    f = 10.0 + 10 // error

    // -
    f = 10.0 - 10.0 // ok
    f = 1 - 2 // ok
    f = false - true // error
    f = 10 - 10.0 // error
    f = 10.0 - 10 // error

    // *
    f = 10.0 * 10.0 // ok
    f = 1 * 2 // ok
    f = false * true // error
    f = 10 * 10.0 // error
    f = 10.0 * 10 // error

    // /
    f = 10.0 / 10.0 // ok
    f = 1 / 2 // ok
    f = false / true // error
    f = 10 / 10.0 // error
    f = 10.0 / 10 // error

    // **
    f = 10 ** 10 // ok
    f = 1 ** 2.0 // error
    f = false ** 1 // error

    // %
    f = 10 % 10 // ok
    f = 1 % 2.0 // error
    f = false % 1 // error

    // > comp
    f = 10.0 > 10.0 // ok
    f = 1 > 2 // ok
    f = false > true // error
    f = 10 > 10.0 // error
    f = 10.0 > 10 // error

    // >= comp
    f = 10.0 >= 10.0 // ok
    f = 1 >= 2 // ok
    f = false >= true // error
    f = 10 >= 10.0 // error
    f = 10.0 >= 10 // error

    // < comp
    f = 10.0 < 10.0 // ok
    f = 1 < 2 // ok
    f = false < true // error
    f = 10 < 10.0 // error
    f = 10.0 < 10 // error

    // <= comp
    f = 10.0 <= 10.0 //ok
    f = 1 <= 2 // ok
    f = false <= true // error
    f = 10 <= 10.0 // error
    f = 10.0 <= 10 // error

    // equivalence comp
    f = false == true // ok
    f = true == 10 // error
    f = 10 == false // error

    // nonequivalence comp
    f = false != true // ok
    f = true != 10 // error
    f = 10 != false // error

    // and comp
    f = false and true // ok
    f = true and 10 // error
    f = 10 and false // error
    
    // or comp
    f = false or true // ok
    f = true or 10 // error
    f = 10 or false // error

    // negation
    f = !true // ok
    f = !10 // error
} 
*/
