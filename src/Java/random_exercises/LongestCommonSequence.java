
public class LongestCommonSequence {

    public String perform(String s1, String s2) {
        int largestSequenceSize;
        String source, target;
        if(s1.length() < s2.length()) {
            largestSequenceSize = s1.length();
            source = s1;
            target = s2;
        }
        else {
            largestSequenceSize = s2.length();
            source = s2;
            target = s1;
        }  
        while(largestSequenceSize > 0){
            int sourceStart = 0;
            while(sourceStart + largestSequenceSize <= source.length()) {
                String subseq = source.substring(sourceStart, sourceStart+largestSequenceSize);
                if(target.indexOf(subseq) >= 0) {
                return subseq;
                }
                ++sourceStart;
            }
            --largestSequenceSize;
        }

        return "";
    }
}