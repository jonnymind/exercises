
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class CommonSubpath {
	
	static class Node {
       String name;
       Node left =null ;
       Node right = null; 

       Node(String name) {
           this.name = name;
       }
	   
        Node(String name, Node left, Node right) {
		   this.name = name;
		   this.left = left;
		   this.right = right;
	   }
    }

    public static String findPathBetween(Node root, String start, String end) {
        List<Node> pathStart = new ArrayList<Node>();
        List<Node> pathEnd = new ArrayList<Node>();
        List<Node> workPath = new ArrayList<Node>();
        
        visit(root, pathStart, pathEnd, workPath, start, end);
        int shortestPathLenght = pathStart.size() < pathEnd.size() ? 
            pathStart.size() : pathEnd.size();
       
        List<Node> common = new ArrayList<Node>();
        for(int i = 0; i < shortestPathLenght; ++ i) {
            if(pathStart.get(i).name != pathEnd.get(i).name) {
                for(int j = pathStart.size()-1; j >= i; --j) {
                    common.add(pathStart.get(j));
                }
                for(int j = i-1; j < pathEnd.size();  ++j) {
                    common.add(pathEnd.get(j));
                }
                break;
            }
        }
        
        return String.join(",", common.stream().map(node -> node.name)
        		.collect(Collectors.toList()));
    }

    public static void visit(Node node, 
        List<Node> pathStart, 
        List<Node> pathEnd,
        List<Node> workPath,
        String start, String end) {

        if(node == null) {
            return;
        }
        
        workPath.add(node);

        if(node.name == start) {
            pathStart.addAll(workPath);
        }

        if(node.name == end) {
            pathEnd.addAll(workPath);
        }

        if(!pathStart.isEmpty() && !pathEnd.isEmpty()) {
            return;
        }

        visit(node.left, pathStart, pathEnd, workPath, start, end);
        visit(node.right, pathStart, pathEnd, workPath, start, end);
        workPath.remove(workPath.size()-1);
    }
    
    public static void main(String args[]) {

        Node d = new Node("D");
        Node e = new Node("E");
        Node f = new Node("F");
        Node g = new Node("G");
        Node b = new Node("B");
        b.left = d;
        b.right = e;
        Node c = new Node("C");
        c.left = f;
        c.right = g;
        Node a = new Node("A");
        a.left = b;
        a.right = c;
        
    	System.out.println(findPathBetween(a, "C", "E"));
    	System.out.println(findPathBetween(a, "A", "A"));
    	System.out.println(findPathBetween(a, "B", "G"));
    }
}
