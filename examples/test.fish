import treenode : "../examples/treenode.fish"

start {
    dec treenode root = new treenode { data: 10, left: empty, right: empty }

    dec treenode a = new treenode { data: 7, left: empty, right: empty }
    dec treenode b = new treenode { data: 60, left: empty, right: empty }
    dec treenode c = new treenode { data: 9, left: empty, right: empty }
    dec treenode d = new treenode { data: 8, left: empty, right: empty }
    dec treenode e = new treenode { data: 11, left: empty, right: empty }

    root...addNode(a)
    root...addNode(b)
    root...addNode(c)
    root...addNode(d)
    root...addNode(e)

    root...inorderTraversal(void)
}
