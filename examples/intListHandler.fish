Uat {
    [int] list
    int size
}

Ufn{
    (fun display_(int i)(void){
        Tree(
            (| i != own.size | { 
                printInt(getAtIndexInt(own.list, i))
                ++i
                own...display_(i)
            })
        )
    })

    (fun display(void)(void){
        printStr("List Contents:")
        own...display_(0)
    })

    # rather inefficient because we malloc every time
    (fun push(int i)(void){
       dec [int] is = [i]
       own.list = appendListInt(own.list, is, own.size, 1)
       ++own.size
    })

    (fun peek_front(void)(int){
      return getAtIndexInt(own.list, 0)
    })


    (fun removeByIndex(int i)(void){
        Tree (
            ( | own.size == 0 | { printStr("Sorry, list is empty!") })
            ( | true | {
                own.list = deleteAtIndexInt(own.list, own.size, i)
                --own.size
            })
        )
    })

    (fun setAtIndex(int i, int v)(void){
       Tree (
           ( | (i > own.size) or (i < 0) | { printStr("Invalid index") } )
           ( | true | {  own.list = setAtIndexInt(own.list, i, v) })
       )
    })
}