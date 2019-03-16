Cat Node {
    Node next
    int data
}

Cfn Node {
    fun has_next
    <- void
    -> bool
    {
        dec bool ret = | own.next != void |

        return ret
    }

    fun set_next
    <- Node node
    -> void
    {
        own.next = node
    }

    fun next
    <- void
    -> Node
    {
    	return own.next
    }

    fun data
    <- void
    -> int
    {
        return own.data
    }
}

Cat Stack {
    Node head
    int size
}

Cfn Stack {
    fun is_empty 
    <- void
    -> bool
    {
        return | own.size == 0 |
    }

    fun size
    <- void
    -> int
    {
        return own.size
    }

    fun peek
    <- void
    -> int
    {
        // since return is at end, must declare a return value here
        dec int i = 0
        if | own...is_empty() == true | {
            // do nothing
        } else {
            // super hacky way of accessing methods on attributes
            dec Node temp = new Node { next: void, data: 0 }
            temp = own.head
            i = temp...data()            
        }

        return i
    }


    fun push
    <- Node node
    -> void
    {
        node...set_next(own.head)
        own.head = node
        own.size = own.size + 1
    }

    fun pop
    <- void
    -> void
    {
        if | own...is_empty() == true | { }
        else {
            dec Node temp = new Node { next: void, data: 0 }
            temp = own.head
            own.head = temp...next()
            
            own.size = own.size - 1 
        }
    }

    fun print_
    <- Node node
    -> void
    {
    	if | node!= void| {
    	    display_int(node...data())
	        display_str(" ")
	        own...print_(node...next())
	    } 
	
	    else {}
    }
    
    fun print
    <- void
    -> void
    {
    	display_str("Stack contents: ")
        own...print_(own.head)	
        display_str("\n")
    }
}


start {
    dec Node a = new Node { next: void, data: 10 }
    dec Node b = new Node { next: void, data: 20 }
    dec Node c = new Node { next: void, data: 30 }
    dec Node d = new Node { next: void, data: 40 }
    dec Node e = new Node { next: void, data: 50 }

    dec Stack s = new Stack { head: void, size: 0 }

    s...push(a)
    s...push(b)
    s...push(c)

    s...print()

    display_str("Size: ")
    display_int(s...size())
    display_str("\tTop: ")
    display_int(s...peek())
    display_str("\n")

    s...pop()
    s...pop()
    s...pop()
    s...pop()
    s...pop()

    display_str("Size: ")
    display_int(s...size())
    display_str("\tTop: ")
    display_int(s...peek())
    display_str("\n")

    s...push(d)
    s...push(e)
    s...push(c)
    s...push(b)

     s...print()
}
