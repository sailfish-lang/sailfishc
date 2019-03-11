start {
    dec list someList <[ int ]> new [ 1, 2, 3]
    dec dictionary someDict <[ int : flt ]> new { 1 : 10.0 }
    dec int i = 1
    dec flt f = 10.0

    // list access
    i = someList[1] // ok
    f = someList[1] // error incorrect value type

    // dictionary access
    f = someDict[1] // ok
    f = someDict[10.0] // error incorrect key type
    i = someDict[1] // error incorrect value type
}