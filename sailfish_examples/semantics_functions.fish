// -------       function  definitions     -------- //
// good
fun good
<- int i, str s
-> flt
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

start {
   // -------       function  calls     -------- //
    dec int i = 10
    //i = good(i, "hello world") // ok
    // i = foo(10, i) // error undefined function
    // i = good(10.0, i) // error arg type mismatch
    // i = good(i) // not enough args
    // i = good(i, i, i) // too many args
}