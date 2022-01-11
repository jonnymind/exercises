import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StripUrlRegex {

    private static Pattern urlSplit = Pattern.compile("(.*)\\?(.*)");
    private static Pattern paramSplit = Pattern.compile("((.*?)=.*?)&|((.*?)=.*)");

    public static String stripUrl(String url) {
        return stripUrl(url, null);
    }
    
    public static String stripUrl(String url, String[] paramsToStrip) {
        // Get the query part of the url.
        Matcher urlMatcher = urlSplit.matcher(url);
        if(!urlMatcher.matches()) {
            return url;
        }

        String addressPart = urlMatcher.group(1);
        String queryPart = urlMatcher.group(2);

        // Pre-load the paramters to strip.
        List<String> lpToStrp = new ArrayList<String>();
        if(paramsToStrip != null) {
            Arrays.stream(paramsToStrip).forEach(param -> lpToStrp.add(param));
        }

        Matcher paramMatcher = paramSplit.matcher(queryPart);
        List<String> lpToKeep = new ArrayList<String>();
        paramMatcher.results().forEach(matcher -> {
            String pname = matcher.group(1) == null ? matcher.group(4) :  matcher.group(2);
            // to be skipped?
            if(!lpToStrp.contains(pname)) {
                lpToStrp.add(pname);
                lpToKeep.add(matcher.group(1) == null ? matcher.group(3) : matcher.group(1));
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