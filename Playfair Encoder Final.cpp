# include <iostream>
# include <string>
#include<cstdlib>

using namespace std;

// All the important variables

char Mat[5][5];                             //The Matrix
string key;                                 //Key to the Playfair Matrix
string used="J";                            //The alphabets that have already been filled in the matrix. J is here to prevent it from entering the matrix, even by accident.
string plain;                               //The plain text, with the spaces, numbers and special characters. Untouched version of what the user wants encoded.
string condensed;                           //The plain text condedsed so that spaces, numbers and special characters are removed and doublets are filled with Q's in between. BALLOONS->BALQLOQONS. Also, has an extra Q at the end if the original text was odd numbered. AND->ANDQ.
string encoded="";                          //Pair-wise encoded version of the condensed text.
string Yin;                                 //Two character long condensed plain text piece
string Yang;                                //Encoded Cipher text piece corresponding to Yin

// All the working functions

string capitalize(string sample)
{

/*     Used in referance from make_Mat() function.
 *     This function takes a string as a parameter, scans it element by element, and capitalizes each element as it goes along.
 *     It returns the capitalized string.
 */

    for(int z=0; z<sample.length(); z++)
    {
        sample[z]= toupper(sample[z]);;
    }

    return sample;
}

int check_used(char chk)
{

/*     Used in referance from make_Mat() function.
 *     This function checks the global string used.
 *     used is a string which starts out with only "J". When we fill an element into the Matrix, it is also added to the used string.
 *     This function takes a character as an argument, and scans the used list for that character. If found, it returns 1. If not, it returns 0.
 */
    int flag=0;

    for(int i=0;i<used.length();i++)
    {
        if(used[i]==chk)
        {
            flag=1;
        }
    }

    return flag;
}

void make_Mat()
{

/*     This function takes a key from you, and adds the entire alphabet to the end of the key.
 *     This is provisionary, in case the key that you enter does not span the alphabet.
 *     It then capitalizes all your alpabets. This is for neatness' sake, and ease of comparison. See capitalize().
 *     Now it will scan your key, element by element. If it encounters any J's, they are replaced by I's.
 *     It will make sure the element being scanned is not already in the Matrix, and that it is infact an alphabet. See check_used(). isalpha() is pre-defined.
 *     If both the above conditions are satisfied, and you have a non redundant alphabet on your hands, it will insert it in the Matrix, going row-wise. It will also add the element to the used list.
 *     In this manner, it systematically fills the entire 5x5 Matrix with all the alphabets excluding J.
 */

    cout<<"Enter the Key:   ";
    getline(cin,key);
    key+="abcdefghijklmnopqrstuvwxyz";
    key=capitalize(key);
    int p=0, q=0;

    for(int a=0;a<key.length();a++)
    {
        if(key[a]=='J')
        {
            key[a]='I';
        }

        if((check_used(key[a])==0)&&(isalpha(key[a])))
        {
            Mat[p][q]=key[a];
            used+=key[a];
            q++;
            if(q>=5)
            {
                q=0;
                p++;

                if(p>=5)
                {
                    break;
                }
            }
        }
    }
}

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

string correctdoublet (string rough)
{

/*     Used in referance from condense() function
 *     It takes a rough string, and adds its elements one by one to the final string.
 *     If the next element and the current element are the same, it adds a Q to the final string before adding the next element.
 *     It returns the final string.
 */

    string final="";
    for(int s=0; rough[s]!='\0';s++)
    {
        final+=rough[s];

        if(rough[s]==rough[s+1])
        {
            final+="Q";
        }
    }

    return final;
}

string condense(string orig)
{

/*     This function takes the plain text as an argument and stores it in the variable orig (short for original).
 *     It creates an empty string, condensed_temp="".
 *     It scans orig, element by element, and pics up all alphabets only. It capitalizes the alphabets and ads them to condensed_temp. If the alphabet happens to be a J, it adds an I instead to condensed_temp.
 *     It then scans the condensed_temp for the occourance of doublets(two alphabets that are the same, adjacent to each other), and adds Q's in between each of the doublets. See correctdoublet().
 *     Finally, it checks whether condensed_temp is odd numbered. If so, it adds a Q at the end of it.
 *     It returns the string condensed_temp.
 */
    string condensed_temp="";
    for (int w=0; w<orig.length(); w++)
    {
        if(isalpha(orig[w]))
        {
            if(toupper(orig[w])=='J')
            {
                condensed_temp += "I";
            }
            else
            {
                condensed_temp +=toupper(orig[w]);

            }
        }

    }

    condensed_temp = correctdoublet(condensed_temp);

    if(condensed_temp.length()%2==1)
    {
        condensed_temp += "Q";
    }

    return condensed_temp;
}

int findX(char A)
{

/*     Used in referance from encode() function
 *     Scans the Matrix for the character stored in A, and returns its row position.
 *     If the character is not found, returns a -1.
 */

    int r,c;
    int foundR = -1, foundC = -1;

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

/*     Used in referance from encode() function
 *     Scans the Matrix for the character stored in A, and returns its column position.
 *     If the character is not found, returns a -1.
 */

    int r,c;
    int foundR = -1, foundC = -1;

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

    cout<<endl<<Yin_temp<<" = "<<Yang_temp<<endl;

    return Yang_temp;
}


// The Main Function

int main()
{
    cout<<endl<<"This program will construct a Playfair Matrix for you based on a keyword or \nkeyphrase that you will provide. It will also encode a message for you, using \nthe constucted Matrix."<<endl<<endl;
    cout<<"NOTE: The conventions used in this program are:"<<endl<<endl;
    cout<<"\t 1.    All the J's are replaced with I's instead of ommitting all \n\t       the Q's from the encoded text."<<endl;
    cout<<"\t 2.    If your message is an odd numbered one, a Q will be added \n\t       at the end of it to even it out."<<endl;
    cout<<"\t 3.    All occourances of doublets are filled with Q's in between.\n\t       Example, balloons --> balQloQons."<<endl<<endl;

    system("PAUSE");
    system("cls");

    make_Mat();
    print_Mat();

    cout<<endl<<"Please enter the text that you want encoded using this matrix:    ";
    getline(cin,plain);

    condensed= condense(plain);

    for(int h=0; h< condensed.length()/2 ; h++)
    {
        Yin=condensed.substr(2*h,2);
        Yang= encode(Yin);
        encoded+=Yang;
    }

    print_Mat();
    cout<<endl<<"This is your condensed plain text:   "<<endl<<condensed<<endl;
    cout<<endl<<"This is your encoded message: "<<endl<<encoded<<endl;

    return 0;
}
