public class IsPerfectTree{
    class TreeNode {
        private TreeNode l, r;
        TreeNode(TreeNode tl, TreeNode tr) {
            l = tl;
            r = tr;
        }

        TreeNode left() {return l;}
        TreeNode right() {return r;}
    }

    public static boolean check(TreeNode node) {
        if(node == null) {
        return true;
    }

    int leftDepth = getLeftDepth(node);
    return isSameDepth(leftDepth, node);
    }

    private static int getLeftDepth(TreeNode node) {
        if(node == null) return 0;
        return getLeftDepth(node.left()) + 1;
    }

    private static boolean isSameDepth(int depth, TreeNode node) {
    if(depth == 0) return node == null;
    if(node == null) return false;
    return isSameDepth(depth - 1, node.left()) && isSameDepth(depth - 1, node.right());
    }
}
