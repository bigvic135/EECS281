Input: character pointers to strings A, B, and C
Output: a bool that reads true only if C is a meshing of A and B

function isMeshed(char∗ A, char∗ B,char∗ C)
    int a = length(A)
    int b = length(b) // get lengths for both strings
    bool arr[a+1, b+1] // create an array of bools
    initFalse(arr) // initialize array to be all false

if (a + b! = length(C)) then
    return false // if A and B don’t sum to the size of C
for i <= a do
    for j <= b do
        if(i == 0 && j == 0) then
            return arr[i, j] = true // if A and B empty
        if (i == 0 && B[j - 1] == C[j  1]) then
            return arr[i, j] = arr[i, j − 1] // if A empty
        if (j == 0 && A[i − 1] == C[i − 1]) then
            return arr[i, j] = arr[i − 1, j] // if B empty
        if (A[i − 1] == C[i + j − 1] && B[j − 1] != C[i + j − 1]) then
            return arr[i, j] = arr[i − 1, j] // ∗A=∗C and ∗B!=∗C
        if (A[i − 1] != C[i + j − 1] && B[j − 1] == C[i + j − 1]) then
            return arr[i, j] = arr[i,j − 1] // ∗A!=∗C and ∗B=∗C
        if (A[i − 1] == C[i + j − 1] && B[j − 1] == C[i + j − 1]) then
            return arr[i − 1, j] || arr[i, j − 1] // ∗A = ∗C and ∗B = ∗C
return arr[a, b]
