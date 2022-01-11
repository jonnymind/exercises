package palindrome;

String LongestPalindrome(String word) 
{
    if(word.length() == 0)
    return "";

    char[] chars = word.toCharArray();
    String candidate = "" + chars[0];

    int beg = 0;
    while(beg < chars.length-1) {
    int end = chars.length-1;
    while(end >= beg + candidate.length()) {
        if(chars[beg] == chars[end]) {
        int pbeg = beg+1;
        int pend = end-1;
        while(pbeg < pend) {
            if(chars[pbeg] != chars[pend]) {
            break;
            }
            pbeg ++;
            pend --;
        }
        // Was this a palinrome?
        if(pbeg >= pend) {
            // We know for a fact the candidate is shorter.
            candidate = word.substring(beg, end+1);
            beg += candidate.length()-1; // the loop already increments.
            break;
        }
        }
        --end;
    }
    beg++;
    }

    return candidate;
}