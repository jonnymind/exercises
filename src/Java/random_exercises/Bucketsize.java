import java.util.Arrays;
import java.util.LinkedList;

public class Bucketsize {

    public static String[] bucketize(String input, int bucketSize) {
        var inputChars = input.toCharArray();
        int startPos = 0;
        int foundLenght = 0;
        int candidateLenght = 0;
        var buckets = new LinkedList<String>();

        while(startPos + foundLenght < inputChars.length) {
            // trim start.
            if(inputChars[startPos] == ' ') {
                startPos ++;
                continue;
            }
            
            if(foundLenght == bucketSize) {
                // We may have a bucket to add.
                if(candidateLenght > 0){
                    buckets.add(input.substring(startPos, startPos + candidateLenght));
                    startPos += candidateLenght +1;
                    candidateLenght = 0;
                    foundLenght = 0;
                }
                // We have a problem.
                else {
                    return new String[0];
                }
            }

            if(inputChars[startPos + foundLenght] == ' ') {
                // we found a probable bucket.
                candidateLenght = foundLenght;
            }
            ++foundLenght;
        }
        if (foundLenght > 0){
            buckets.add(input.substring(startPos, startPos+foundLenght));
        }

        // Convert to string array
        var ret = new String[buckets.size()];
        for(int i = 0; i < ret.length; ++i) {
            ret[i] = buckets.getFirst();
            buckets.removeFirst();
        }
        return ret;
    }

    public static void main(String[] args) {
        System.out.println(Arrays.toString(bucketize("she sells sea shells by the sea", 10)));
        System.out.println(Arrays.toString(bucketize("the mouse jumped over the cheese", 7)));
        System.out.println(Arrays.toString(bucketize("fairy dust coated the air", 20)));
        System.out.println(Arrays.toString(bucketize("a b c d e", 2)));
        System.out.println(Arrays.toString(bucketize("ab cde fg hij", 2)));
        System.out.println(Arrays.toString(bucketize("abc def", 2)));

    }
}