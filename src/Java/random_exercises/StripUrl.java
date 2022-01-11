import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class StripUrl {

    public static String stripUrl(String url) {
        return stripUrl(url, null);
    }
    
    public static String stripUrl(String url, String[] paramsToStrip) {
        // Get the query part of the url.
        int posQuery = url.indexOf('?');
        if(posQuery == -1 || posQuery == url.length() - 1) {
            // nothing to do.
            return url;
        }

        String addressPart = url.substring(0, posQuery);
        String queryPart = url.substring(posQuery+1);

        // Pre-load the paramters to strip.
        List<String> lpToStrp = new ArrayList<String>();
        if(paramsToStrip != null) {
            Arrays.stream(paramsToStrip).forEach(param -> lpToStrp.add(param));
        }

        String[] paramsAndValues = queryPart.split("&");
        List<String> lpToKeep = new ArrayList<String>();
        Arrays.stream(paramsAndValues).forEach(param -> {
            int pnamePos = param.indexOf('=');
            String pname;
            if(pnamePos != -1) {
                pname = param.substring(0, pnamePos);
            }
            else {
                pname = param;
            }

            // to be skipped?
            if(!lpToStrp.contains(pname)) {
                lpToStrp.add(pname);
                lpToKeep.add(param);
            }
        });

        // rebuild the URL.
        if(lpToKeep.isEmpty()) {
            return addressPart;
        }
        
        return addressPart + "?" + String.join("&", lpToKeep);
    }

    public static void main(String args[]) {
        System.out.println(stripUrl("https://edabit.com?a=1&b=2&a=2"));
        var cutb = new String[]{"b"};
        System.out.println(stripUrl("https://edabit.com?a=1&b=2&a=2", cutb));
        System.out.println(stripUrl("https://edabit.com", cutb));
    }
}