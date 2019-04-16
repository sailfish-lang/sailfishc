Uat {
    [int] list
    int size
}

Ufn {
    (fun dispSize(void)(void){
        printInt(own.size)
    })

    (fun incrementSize(void)(void){
        ++own.size
    })

    (fun decrementSize(void)(void){
        --own.size
    })
}