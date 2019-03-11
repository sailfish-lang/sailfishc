start {
    // list assignment
    dec list some_list <[ int ]> new [ 1, 2, 3 ]
    // some_list = [ 4, 5, 6, 7 ]

    // dictionary assignment
    dec dictionary some_dict <[ int : flt ]> new { 2 : 20.0 }
    // some_dict = { 1: 10.0 }

    // string literal
    dec str some_str = "hello world"
    some_str = "goodbye town"

    // integer literal
    dec int some_int = 1
    some_int = 2

    // float literal
    dec flt some_flt = 10.0
    some_flt = 20.0

    // boolean literal
    dec bool some_bool = true
    some_bool = false
}