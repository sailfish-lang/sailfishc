/* Entirety here is source, made of source parts */

// --------        Export Definition      ------- //
// exp dec flt someFlt = 10.0
// exp dec list someList = new [1,2,3]
// exp dec dictionary someDict = new {1 : "hello", 2: "world"}

// -------       General Decleration       ------- //
// dec flt someNonExportFlt = 10.0
// dec list someNonExportList = new [1,2,3]
// dec dictionary someNonExportDict = new {1 : "hello", 2: "world"}

// -------       Function Definition       ------- //
/*
    fun foo
        <- int i, flt f, str s
        -> str, float
    {}
*/

// -------       User Defined Type Definition       ------- //
/*
    Cat FooS {
            int someInt
            flt someFloat
            str SomeString
    }

    Cfn FooS {
   
        fun bar
            <- void
            -> void
            {}

        fun baz
            <- int inty, void, flt flty
            -> void
            {}
    }
*/

// -------       Initial Execution Body       ------- //
start {}