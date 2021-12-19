int rassadka228MLG1337(int curPair, int** arr, int** vragi, int m)
{
    for (int i = curPair; i < m; ++i)
    {
        int vrag1 = vragi[i][0];
        int vrag2 = vragi[i][1];

        if (arr[1][vrag1] == 0)
            if(arr[1][vrag2] == 0)
                {
                    arr[1][vrag1] = 1;
                    arr[1][vrag2] = -1;
                    if(!rassadka228MLG1337(i+1, arr, vragi, m))
                    {
                        arr[1][vrag1] = -1;
                        arr[1][vrag2] = 1;
                        return rassadka228MLG1337(i+1, arr, vragi, m);
                    }
                }
            else
            {
                arr[1][vrag1] = -arr[1][vrag2];
            }
        else if (arr[1][vrag2] == 0)
            {
                arr[1][vrag2] = -arr[1][vrag1];
            }
        else if (arr[1][vrag2] == arr[1][vrag1]) {return 0;}
    }
    return 1;
}


int main() {
    
    return 0;
}