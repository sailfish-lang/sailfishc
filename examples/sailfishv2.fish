Uat Node {
    Node next
    int data
}

# This is a comment
Ufn Node {
    (fun next(void)(Node)
    (own.next = 10, return own.next))
}

start (
    # This is also a comment
    dec int i = 10
    i += (10 - 11)
)
