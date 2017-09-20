#include<iostream>
#include<string>
#include<cstdlib>

using namespace std;

//Important Global Variables

string PT,CT;                           // Plain Text, Cipher text (i.e Condensed plain text, so it sould be of the same length as CT)
string Quadmap="";                      // Long list containing groups of 4 alphabets in PTCT format
string trio_list="";                    // Long list containing groups of 3 alphabets that are linearly adjacent in the matrix
string  quartet_list="";                // Long list containing groups of 4 alphabets that are linearly adjacent in the matrix
char Mat[5][5];                         // The Matrix
string piece="";                        // Provisional string
int Redflag=0;                          // Error flag
string filled="";                       // Used to facilitate the LastResort() function
char temp[5][5];                        // Used to facilitate the transpose() function
int index[2][2];                        // Used to facilitate the UseRectangles2() function. This stores the indices of the letters of the plaintext in the key in order i.e index[0][0] has the
                                        // the ith and index[0][1] the jth position of the first letter. similar for the second letter

// Functions essential to the extraction of the Matrix

int Quadmap_contains(string QUAD)
{

/*     Used in referance from initialize() function.
 *     This function scans the Quadmap already in place for the occourance of the quad passes as the parameter.
 *     This function helps get rid of the redundancies that we many encounter in the data set, thus making the Extractor more efficient.
 *     Returns 1 if quad already in Quadmap, Returns 0 otherwise.
 */
    int contained=0;

    for(int i3=0; i3< Quadmap.length()/4 ; i3++)
    {
        if(Quadmap.substr(4*i3,4)==QUAD)
           {
               contained=1;
               break;
           }
    }

    return contained;
}

int contains_trio(string QUAD)
{

/*     Used in reference from initialize() function.
 *     If an alphabet occours twice in a quad, then the tree unique alphabets in the quad must form a trio.
 *     If the quad mapping is such that the pieces occour in the order they should appear in the matrix, then it is refered to as a straight trio. Eg: ABBC => ABC is a trio
 *     If the quad mapping is such that the pieces do not occour in the order they should appear in the matrix, then it is refered to as a skew trio. Eg: BACB => ABC is a trio
 *     All other arrangements of 4 alphabets such that 2 are repeated, violate the laws of Playfair, and cannot occour in a quad.
 *     If the quad contains a straight trio, then this function returns a 1.
 *     If the quad contains a straight trio, then this function returns a -1.
 *     If the quad does not contain a trio, then this function returns a 0.
 */

    if(QUAD[1]==QUAD[2])
    {
        return 1;
    }

    else if(QUAD[0]==QUAD[3])
    {
        return -1;
    }

    else
    {
        return 0;
    }
}


void initialize()
{

/*     This function takes PT and CT pieces in corresponding pairs of two and joins them together to make a quad.
 *     It stores all the new and non-redundant quads in a long string of quads called Quadmap. See Quadmap_contains().
 *     If the quad is non-redundant, it checks the quad for the occourance of a trio (a group of 3 alphabets that are linearly adacently in the Matrix). See contains_trio().
 *     If it finds that the quad contains a trio, it will add the trio, to a long string of trios called trio_list.
 */

    for(int i2=0; i2< PT.length()/2; i2++)
    {
        string quad="";
        string quad2="";        //This will be made the mirror equivalent of quad, which shall also be checked by Quadmap_contains(), further ensuring no redundancies enter our Quadmap

        quad+=PT.substr(2*i2,2);
        quad+=CT.substr(2*i2,2);

        quad2+=quad[1];
        quad2+=quad[0];
        quad2+=quad[3];
        quad2+=quad[2];

        if((Quadmap_contains(quad)==0)&&(Quadmap_contains(quad2)==0))
        {
            Quadmap+=quad;

            if(contains_trio(quad)==1)
            {
                trio_list+=quad[0];
                trio_list+=quad[1];
                trio_list+=quad[3];
            }

            if(contains_trio(quad)==-1)
            {
                trio_list+=quad[1];
                trio_list+=quad[0];
                trio_list+=quad[2];
            }
        }

    }
}

void find_quartetlist()
{

/*     This function is designed to use the trio_list, and from it derive quartets.
 *     Quartets are groups of 4 alphabets that are linearly adjacent in the Matrix.
 *     Once found, these quartets are store in a long list called quartet_list, similar to trio_list.
 */

    quartet_list="";
    string quartet="";

    for(int i4=0; i4<  trio_list.length()/3 ;  i4++)
    {
        string strio;
        strio=trio_list.substr(3*i4,3);
        for (int i5=i4+1; i5< trio_list.length()/3; i5++)
        {
            string ctrio;
            ctrio=trio_list.substr(3*i5,3);

            if(strio.substr(1,2)==ctrio.substr(0,2))    //Do the last two alphabets of strio match with the first two alphabets of ctrio
            {
                quartet = strio + ctrio[2];
                quartet_list += quartet;
            }

            else if(ctrio.substr(1,2)==strio.substr(0,2))    //Do the last two alphabets of ctrio match with the first two alphabets of strio
            {
                quartet = ctrio + strio[2];
                quartet_list += quartet;
            }
        }
    }
}

void fillwithnull()
{

/*     This just fills the entire matrix with #'s
 *     Comaprisons with the actual characters are in my experience, far more reliable than comparisons with \0
 */

    for(int k1=0; k1<5; k1++)
    {
        for(int k2=0; k2<5; k2++)
        {
            Mat[k1][k2]='#';
        }
    }
}

int contained_at(char T, string word)   // T for Target alphabet
{

/*     Used in referance from fillmatrix() function.
 *     Scans a word for a Target alphabet. If the alphabet is found, it returns the index of the alphabet in the word.
 *     If the alphabet is not found, it returns -1.
 */

    int poz=-1;
    for(int k=0; k<word.length(); k++)
    {
        if(word[k]==T)
        {
            poz=k;
            break;
        }
    }

    return poz;
}


int Horizontal_fit(int I, int J, int POS, int n)
{

/*     Used in referance from the fillmatrix() function.
 *     Refers the global string piece.
 *     Checks if piece fits Horizontally with the element at <I,J> in the matrix.
 *     Returns 1, if it's sure that piece fits horizontally at <I,J>.
 *     Returns -1, if it's sure that piece fits vertically at <I,J>.
 *     Returns 0, if can't be sure of either.
 */

    int H=0;

    if(POS==0)                                              //If we are dealing with the left most element of the piece
    {
        if(piece[POS+2] == Mat[I][(J+5+2)%5])               //Check for consistancy two spaces to the right of the element.
        {
            H=1;
        }

        else if( Mat[I][(J+5+2)%5] !='#')                   //If inconsistant, was the inconsistancy because of anything other than a null space
        {
            H=-1;
        }

        else if(piece[POS+2]== Mat[(I+5+2)%5][J])           //If it was a null space that caused the inconsistancy, check for vertical consistancy (below)
        {
            H=-1;
        }

        // If none of the above conditions were satisfied, then we can't be sure what to do with the piece. H remains 0.
    }

    if(POS==n-1)                                                //If we are dealing with the right most element of the piece
    {
        if(piece[POS-2] == Mat[I][(J+5-2)%5])                   //Check for consistancy two spaces to the left of the element.
        {
            H=1;
        }

        else if( Mat[I][(J+5-2)%5] !='#')                       //If inconsistant, was the inconsistancy because of anything other than a null space
        {
            H=-1;
        }

        else if(piece[POS-2]== Mat[(I+5-2)%5][J])               //If it was a null space that caused the inconsistancy, check for vertical consistancy (above)
        {
            H=-1;
        }

        // If none of the above conditions were satisfied, then we can't be sure what to do with the piece. H remains 0.
    }

    else                                                                                    //If we are dealing with an element central to the given piece
    {
        if((piece[POS+1] == Mat[I][(J+5+1)%5]) || (piece[POS-1] == Mat[I][(J+5-1)%5]))      //check whether there is a left OR right correspondance in the matrix with the piece
        {
            H=1;
        }

        else if((( Mat[I][(J+5+1)%5])!='#') || ((Mat[I][(J+5-1)%5])!='#'))                  //if not, then did this inconsistancy arrise from anything other than a null
        {
            H=-1;
        }

        else if((piece[POS+1] == Mat[(I+5+1)%5][J]) || (piece[POS-1] == Mat[(I+5-1)%5][J])) //if it was an inconsistancy because of surrounding null spaces, then check above and below for consistantcies
        {
            H=-1;
        }

        // If none of the above conditions were satisfied, then we can't be sure what to do with the piece. H remains 0.
    }

    return H;
}

void fillmatrix(string list2fill, int N)
{
/*     This function is designed so that it can fill the quartet_list, as well as the trio_list into the matrix, by fitting them piece-wise.
 *     The list2fill argument will take either the trio_list, or the quartet_list, and N will be the corresponding order of that list.(3 for trio_list, 4 for quartet_list)
 *     First, if there is at least one quartet, this function will fill the first 4 slots in the top row with it.
 *     Else, if there is at least one tripplet this function will fill the first 3 slots in the top row with it.
 *     The function scans the matrix, element by element, row-wise. It skips over the #, or unfilled spaces.
 *     If it encounters a filled space (not #) at <i,j>, then it scans the list2fill for pieces that have the alphabet found at <i,j>. See contained_at().
 *     Once the appropriate piece is found, it checks whether the piece fits horizontally with that cell in the matrix. See Horizontal_fit().
 *     If it does fit Horizonatally, it is inserted horizontally using the appropriate wrap around condition.
 *     Else if it fits Vertically, it is inserted vertically using the appropriate wrap around condition.
 *     This is repeated untill every piece that has the alphabet stored in <i,j>, is appropriately fit around <i,j>.
 *     Then we move to the next slot in the matrix, and repeat this until the entire matrix has been spanned.
 */


    if(quartet_list.length()!= 0)
    {
        Mat[0][0]=quartet_list[0];
        Mat[0][1]=quartet_list[1];
        Mat[0][2]=quartet_list[2];
        Mat[0][3]=quartet_list[3];
    }

    else if(trio_list.length()!=0)
    {
        Mat[0][0]=trio_list[0];
        Mat[0][1]=trio_list[1];
        Mat[0][2]=trio_list[2];
    }
    else
    {
        Redflag=1;                              //This is a flag used to tackle possible run time errors. Redflag=1 => No quartets nor trios. Totally insufficient data, no part of the matrix can be extracted.
    }


    for(int k3=0; k3<5; k3++)
    {
        for(int k4=0; k4<5; k4++)
        {
            if(Mat[k3][k4] != '#')
            {
                for(int k5=0; k5< list2fill.length()/N ; k5++)
                {
                    piece=list2fill.substr(N*k5, N);

                    if( contained_at( Mat[k3][k4], piece) != -1 )
                    {
                        int pos= contained_at( Mat[k3][k4], piece);

                        if( Horizontal_fit(k3, k4, pos, N) == 1)                // is it fitting Horizontally
                        {
                            for(int k6=0; k6<N ; k6++)
                            {
                                Mat[k3][(k4+5-pos+k6)%5]=piece[k6];
                            }
                        }

                        else if(Horizontal_fit(k3, k4, pos, N) == -1)           // is it fitting Vertically
                        {
                            for(int k6=0; k6<N; k6++)
                            {
                                Mat[(k3+5-pos+k6)%5][k4]=piece[k6];
                            }
                        }
                    }
                }
            }
        }
    }
}

int findX(char A)
{

/*     Used to find the Row of the Character passed in the argument A.
 *     Returns the Row index of the character stored in A, in the Playfair Matrix.
 *     Retruns -1 if the character stored in A is not found in the Matrix.
 */

    int r,c;
    int foundR=-1,foundC=-1;

    for(r=0;r<5;r++)
    {
        for(c=0;c<5;c++)
        {
            if(A==Mat[r][c])
            {
                foundR=r;
                foundC=c;
            }
        }
    }

    return foundR;
}

int findY(char A)
{
/*     Used to find the Column of the Character passed in the argument A.
 *     Returns the Coloumn index of the character stored in A, in the Playfair Matrix.
 *     Retruns -1 if the character stored in A is not found in the Matrix.
 */
    int r,c;
    int foundR=-1, foundC=-1;

    for(r=0;r<5;r++)
    {
        for(c=0;c<5;c++)
        {
            if(A==Mat[r][c])
            {
                foundR=r;
                foundC=c;
            }
        }
    }

    return foundC;
}


int redundant(string tukda)
{

/*     Used in referance from the refresh() function.
 *     Returns the number of redundant Characters in a Tukda from the Quadmap.
 */

    int cnt=0;

    for(int p2=0; p2<4; p2++)
    {
        for(int p3=0; p3<5; p3++)
        {
            for(int p4=0; p4<5; p4++)
            {
                if(tukda[p2]==Mat[p3][p4])
                {
                    cnt++;
                }
            }
        }
    }

    return cnt;
}

int linearity(string tukda)
{
/*     Used in referance from the refresh() function.
 *     Checks if 3 of the elements in a tukda lie in the same row or column in the Matrix.
 *     Returns 1 if they do. Returns 0 if they don't.
 */

     int linear=0;

     int R=findX(tukda[0]);
     int C=findY(tukda[0]);

     if( (R==-1) || (C==-1) )           //if the first element of tukda has not yet been placed in the matrix, then we can't use it's coordinates as referance
     {
         R=findX(tukda[1]);
         C=findY(tukda[1]);

     }

     int inSameRow=0;
     int inSameCol=0;

     for(int p5=0; p5<4; p5++)
     {
         if(R==findX(tukda[p5]))
         {
             inSameRow++;
         }

         if(C==findY(tukda[p5]))
         {
             inSameCol++;
         }
     }

     if(( inSameRow < 3 )&&( inSameCol < 3 ))
     {
         return 0;
     }

     else
     {
         return 1;
     }

}

void refresh()
{

/*     This function refreshes the Quadmap by getting rid of all the pieces in the Quadmap that are no longer of any use to us.
 *     It selectively keeps those Quads that do not lie in a row or a coloumn, and have exactly
 *     3 redundant elements, and 1 element not yet inserted in the matrix. This must be a Quad that
 *     forms a Rectangle. Of this rectangle, we have 3 vetices. We can find the coorinates of these 3
 *     vertices using our findX and findY functions, and from them, infer the coordinates of the oddman.
 *     (The oddman is the element in the Quad that still needs to be inserted in the matrix)
 */

    string temp_quadmap="";

    for(int p1=0; p1< Quadmap.length()/4 ; p1++)
    {
        string Tukda="";
        Tukda= Quadmap.substr(4*p1,4);

        if(redundant(Tukda)==3)                     //if the tukda has 3 redundant characters => 1 non-redundant character
        {
            if(linearity(Tukda)==0)                 //if the tukda is a non-linear mapping => a Rectangular mapping
            {
                temp_quadmap += Tukda;
            }
        }
    }

    Quadmap = temp_quadmap;
}

char findoddman(string wrd)
{

/*     Used in referance from the UseRectangles() function.
 *     Accepts a Quad as wrd. Finds the one character in the Quad
 *     that hasn't already been inserted in the Matrix, and returns it.
 */

    char ODD;
    for(int p5=0; p5<4; p5++)
    {
        if(findX( wrd[p5] )==-1)
        {
            ODD= wrd[p5];
        }
    }

    return ODD;

}

void UseRectangles()
{

/*     This function simply uses the first Quad from the refreshed list,
 *     and determines the oddman. Then from the redundant 3 in the quad,
 *     it figures out where to insert the oddman, and inserts it. It then
 *     refreshes and repeats the process until there is nothing left of
 *     the Quadmap.
 */

    int Is[3], Js[3];
    int UniqueX, UniqueY;

    while(Quadmap.length() != 0)
    {
        string Word;
        char oddman;

        Word=Quadmap.substr(0,4);
        oddman= findoddman(Word);

        int p6, p7=0;

        for(p6=0; p6<4; p6++)
        {
            if(Word[p6] != oddman)
            {
                Is[p7]=findX(Word[p6]);
                Js[p7]=findY(Word[p6]);
                p7++;
            }
        }

        if( (Is[0]==Is[1])&&(Is[0]!=Is[2]) )
        {
            UniqueX=Is[2];
        }

        else if( (Is[0]!=Is[1])&&(Is[0]==Is[2]) )
        {
            UniqueX=Is[1];
        }
        else if( (Is[0]!=Is[1])&&(Is[0]!=Is[2]) )
        {
            UniqueX=Is[0];
        }


        if( (Js[0]==Js[1])&&(Js[0]!=Js[2]) )
        {
            UniqueY=Js[2];
        }

        else if( (Js[0]!=Js[1])&&(Js[0]==Js[2]) )
        {
            UniqueY=Js[1];
        }
        else if( (Js[0]!=Js[1])&&(Js[0]!=Js[2]) )
        {
            UniqueY=Js[0];
        }



        Mat[UniqueX][UniqueY] = oddman;
        refresh();
    }
}

int search(string PT_pair)  // Function to search whether the letters of the plaintext pair is present in the key matrix
{
    int Flag=0;     //Flag =2 would means that both the letters of the plaintxt are present in the key matrix.
    for(int I2=0;I2<5;I2++)
    {
        for(int J2=0;J2<5;J2++)
        {
            if(Mat[I2][J2]==PT_pair[0])
             {
                 index[0][0]=I2;    //stores x1
                 index[0][1]=J2;   // stores y1
                 Flag+=1;
             }

           if(Mat[I2][J2]==PT_pair[1])
              {
                 index[1][0]=I2;     //stores x2
                 index[1][1]=J2;     // stores y2
                 Flag+=1;
              }
        }
     }

    if((index[0][0]!=index[1][0]) && (index[0][1]!=index[1][1]))// ensuring the PT pair is nnot in the same row or column
    {
        return Flag;
    }
    else
    {
        return 0;
    }

}

void UseRectangles2()
{
  for(int k9=0;k9<PT.length()/2;k9++)
     {
          string pt_pair=PT.substr(k9*2,2);    // extracting pairs from PT and CT
          string ct_pair=CT.substr(k9*2,2);

        // ct_pair[0]=(x1,y2) and ct_pair[1]=(x2,y1) as said above
         if(search(pt_pair)==2)     // search() returns the value of flag and flag=2 means the PT pair is there in the key and
         {                         // are present in distinct columns and rows
              int I1=0,J1=0;
              I1=index[0][0];
              J1=index[1][1];
              Mat[I1][J1]=ct_pair[0];
              I1=0; J1=0;
              I1=index[1][0];
              J1=index[0][1];
              Mat[I1][J1]=ct_pair[1];
         }
     }
}

int remaining()
{

/*     Used in referance from LastResort() function.
 *     Scans the Matrix. Counts the number of #'s (i.e null spaces)
 *     Returns the count.
 */
    int nulls=0;

    for(int p8=0; p8<5; p8++)
    {
        for(int p9=0; p9<5; p9++)
        {
            if(Mat[p8][p9]=='#')
            {
                nulls++;
            }
        }
    }

    return nulls;
}


int scanfilled(char suspect)
{

/*     Used in referance with the LastResort() function.
 *     Scans the filled list for an alphabet passed as suspect
 *     If the alphabet is found, it returns 1, else it returns 0.
 */

    int found=0;

    for(int q4=0; q4<filled.length(); q4++)
    {
        if(suspect==filled[q4])
        {
            found=1;
            break;
        }
    }

    return found;
}

void LastResort()
{
/*     Last Desparate attempt at filling the matrix.
 *     If only one alphabet is left unfilled, it will first mark out all the alphabets to be filled [A to Z without J].
 *     It will then scan the matrix and store all the alphabets filled in it (Effectively ignoring the # case), in a string called filled.
 *     Then it will take the tobefilled list element by element, and see if each element is in the filled list. If one is found that is not,
 *     then it is filled in place of the # in the Matrix.
 */

    if(remaining()==1)
    {
        int Lastx, Lasty;

        string tobefilled="ABCDEFGHIKLMNOPQRSTUVWXYZ";
        filled="";
        char unfilled;

        Lastx= findX('#');
        Lasty= findY('#');

        for(int q1=0; q1<5; q1++)
        {
            for(int q2=0; q2<5; q2++)
            {
                if(Mat[q1][q2]!='#')
                {
                    filled += Mat[q1][q2];
                }
            }
        }

        for(int q3=0; q3<tobefilled.length(); q3++)
        {
            if(scanfilled(tobefilled[q3])==0)
            {
                unfilled=tobefilled[q3];
                break;
            }
        }

        Mat[Lastx][Lasty]=unfilled;
    }
}

string encode (string Yin_temp)
{

/*     Takes Yin(Plain text pair), and according to the rules of Playfair
 *     encodes it to Yang (corresponding Cipher text pair).
 *     It uses the findX(), findY() functions to get the coordinates of an alphabet that is passed as a parameter to either of these two functions.
 *     It uses %5 to ensure wrap around correction in case of index overflow.
 *     It returns Yang
 */

    string Yang_temp="";

    int x1= findX(Yin_temp[0]);
    int y1= findY(Yin_temp[0]);

    int x2= findX(Yin_temp[1]);
    int y2= findY(Yin_temp[1]);

    if(x1==x2)
    {
        Yang_temp+=Mat[x1][(y1+1)%5];
        Yang_temp+=Mat[x2][(y2+1)%5];
    }

    else if(y1==y2)
    {
        Yang_temp+=Mat[(x1+1)%5][y1];
        Yang_temp+=Mat[(x2+1)%5][y2];
    }

    else
    {
        Yang_temp+=Mat[x1][y2];
        Yang_temp+=Mat[x2][y1];
    }

    return Yang_temp;
}

// Housekeeping Functions

void print_Mat()
{

/*
 *     It's as simple as the name suggests. It prints out the matrix (in a row-wise fashion).
 */

    cout<<endl<<"This is your Playfair Matrix: "<<endl<<endl;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            cout<<Mat[i][j]<<"\t";
        }

        cout<<endl;
    }
}

void transpose()
{
    for(int k7=0; k7<5; k7++)
    {
        for(int k8=0; k8<5; k8++)
        {
            temp[k8][k7]=Mat[k7][k8];
        }
    }

    for(int k7=0; k7<5; k7++)
    {
        for(int k8=0; k8<5; k8++)
        {
            Mat[k7][k8]=temp[k7][k8];
        }
    }
}

// The Main Function

int main()
{
    PT="THISISANEXPERIMENTALRUNTOTESTQTHEQEFQFIUCIENCYOFPARTHANDANGADSUSERECTANGLESFUNCTIONASOPQPOSEDTOMINEIFWEARERIGHTQTHEIRFUNCTIONSHOULDBEABLETOEXTRACTFARMOREOFTHEMATRIXTHANMINEHASBEQENABLETOITLOQOKSLIKEWEAREGOINGTONEQEDAMUCHWIDERPOQOLOFDATATHANIANTICIPATEDINMYFIRSTRUNIAMNOWADQDINGALQLOFTHISLETUSQSEQEWHATHAPQPENSQ";
    CT="MBNPNPSAQIILDAQLBYPGGZBYFKOIUTMBQXFGTEAQDSFIBVLGISBURPIBSAURCIKAGDODUNAFEFNOTABKSEASCKIMSLIOBQLKNAQDLYGIDGDALRUTMBQDBGTABKSEAPCLMGBRGIHFFQLFYQGRBKGNHUGCFLTYDLUPUBDIMBSAQPIFRPNCQXFINREFKFNQELKEVCEPQOXLRGFOESAFKFIFXQRIQKHDXPEQHAEKLELGRIUNMBSANSBYSDNINUQENATWENCAUBTANSTPLVIRXENAUREMELTYDPPOFQKAKIQXLXRPMBSIMIFIIK";

    initialize();
    find_quartetlist();
    fillwithnull();
    fillmatrix(quartet_list,4);
    fillmatrix(trio_list, 3);
    refresh();
    UseRectangles();
    UseRectangles2();
    LastResort();

    if(CT.substr(0,2)!=encode(PT.substr(0,2)))
    {
        transpose();
    }

    print_Mat();
}
