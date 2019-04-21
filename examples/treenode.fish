Uat {
    int data
    treenode left
    treenode right
}

Ufn{
   (fun hasLeft(void)(bool) {
       Tree ( (| own.left == empty | { return false } ))
       return true
   })

    (fun hasRight(void)(bool) {
       Tree ( (| own.right == empty | { return false } ))
       return true
   })

   (fun setLeft(treenode tn)(void) {
       own.left = tn
   })

   (fun setRight(treenode tn)(void) {
       own.right = tn
   })

   (fun addNode(treenode tn)(void) {
       Tree (
           ( | (tn.data < own.data) and (!own...hasLeft(void)) | {
               own.left = tn
           })

           ( | (tn.data < own.data) and (own...hasLeft(void)) | {
               own.left...addNode(tn)
           })

           ( | (tn.data >= own.data) and (!own...hasRight(void)) | {
               own.right = tn
           })

           ( | (tn.data >= own.data) and (own...hasRight(void))  | {
               own.right...addNode(tn)
           })
       )
   })

    (fun inorderTraversal(void)(void) {
        Tree (
            # left 
            ( | own...hasLeft(void) | { 
                own.left...inorderTraversal(void)
            })
        )

        # root
        printInt(own.data)

        Tree (
            # right 
            ( | own...hasRight(void) | { 
               own.right...inorderTraversal(void)
            })
       )
   })
}