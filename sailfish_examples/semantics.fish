/*
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
*/

// -------       function      -------- //
/*
// good
fun good
<- void
-> void
{}

// input describes nonexistent type
fun inputDoesNotExist
<- nonExistentType foo
-> void
{}

// input name is a reserved word
fun reserevedKeywordAsInputName
<-flt flt
-> void
{}

// output describes a nonexistent type
fun outputDoesNotExist
<- void
-> nonExistentType
{}

// function name is a reserved type
fun flt
<- void
-> void
{}
*/

// --------       declerations         --------
/*
dec flt a = 10 // good
dec fat b = 10 // error type does not exist
dec flt flt = 10.0 // error name is a primitive
dec flt dec = 10.0 // error name is a keyword
dec flt foo = 10 // error expression type and assignment type are different
*/

// --------       dictionary        -------
/*
dec dictionary a <[ int : flt ]> new { 1 : 1.0 } // good
dec dictionary ab <[ Foo : flt ]> new { 1 : 1.0 } // error key type does not exist
dec dictionary abb <[ int : Foo ]> new { 1 : 1.0 } // error value type does not exist
dec dictionary flt <[ int : flt ]> new { 1 : 1.0 } // error name is reserved
dec dictionary abbb <[ int : flt ]> new [ 1 ] // error type is not a dictionary
*/

// ---------        list         ---------
/*
dec list a <[ int ]> new [ 1 ] // good
dec list ab <[ Foo ]> new [ 1 ] // error key type does not exist
dec list flt <[ int ]> new [ 1 ] // error name is reserved
dec list abbb <[ int ]> new { 1 : 1.0 }  // error type is not a list
*/

// --------       udt       --------
/*
// good
Cat Foo {
    int i
    flt f
    str s
}

Cfn Foo {
    fn foo
    <- void
    -> void
    {}
}

// errors
Cat flt  { 
    flt flt // illegal name
    void // illegal type
    udt u // unknown type
}

Cfn flt {}
*/

start {
    // --------       list literal         ---------
    z = [ 1, 2 ]
}

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
