#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define CR        0x0d  //carriage return
#define TAB       0x09
#define LF        0x0a  //line feed
#define BACKSPACE 0x08
#define NUL       '\0'  //empty character
#define QUIT      1
#define TRUE      1
#define FALSE     0
#define LENGTH    12    //size of the string

int input(char *string, int length);

double multiply(char *, int , int, int, double[*][*]);

void print_table(int , int , char , char);

void Clear_previous_messages(COORD,HANDLE);

COORD Handle_input(char *);

double median(char *, int, int, int, double[*][*]);

void Add_num_to_array(char *, int , int , double[*][*], COORD, COORD, HANDLE);

int table_width = 0, table_height = 0;

double add(char *, int , int , int , double[*][*]);

double average(char *, int , int , int , double[*][*]);

double min_value(char *str,int k,int a, int b,double c[a][b]);

double max_value(char *str,int k,int a, int b,double c[a][b]);

void print_result(int, char *, double, HANDLE, COORD);

void visual_clear(COORD, HANDLE);


int main(void)
{

    //Ορισμός και αρχικοποίηση συγκεκριμένων μεταβλητών


    int exit_flag = 1;
    char string[LENGTH];
    float a;
    char str[20];
    double d;
    int flag_2 = 1;
    int counter = 0;

    //Λαμβάνονται οι κατάλληλες πληροφορίες για το παράθυρο στην αρχή του
    //προγράμματος μέσω των βιβλιοθηκών windows.h και winuser.h


    HANDLE hwin = GetConsoleWindow();                   //https://docs.microsoft.com/en-us/windows/console/getconsolewindow
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);      //https://docs.microsoft.com/en-us/windows/console/getstdhandle
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD InputRecord;
    DWORD fdwSaveOldMode;
    DWORD Events;
    DWORD fdwNewMode = ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    COORD coord;
    COORD inserted_coord;
    COORD default_coord;
    COORD cursor_temp_coord;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD Screen_buffer_size = {2000,2000};
    SetConsoleScreenBufferSize(hout,Screen_buffer_size);

    //Το παράθυρο γίνεται Fullscreen

    ShowWindow(hwin,3);

    //Λαμβάνονται οι διαστάσεις του πίνακα

    printf("Please insert the table dimensions in the form [width]x[height]\n");
    scanf("%dx%d", &table_width, &table_height);
    fflush(stdin);
    COORD beginning = {0,0};

    //Έλεγχος ότι ο χρήστης εισάγει σωστές και δεκτές τιμές

    while(flag_2)
    {
        if((table_width <= 0 || table_width > 25) || (table_height <= 0 || table_height > 25) )
        {

             SetConsoleCursorPosition(hout,beginning);
             printf("                                                                      \n\
                                                                                            ");
             SetConsoleCursorPosition(hout,beginning);
             printf("Please enter integer values between 1 and 25\n");
             scanf("%d x %d", &table_width, &table_height);
        }else
        {flag_2 = 0;}
    }
    default_coord.X = 0;
    default_coord.Y = 5 + 2*(table_height+1);
    double Actual_Table[table_height][table_width];
    char column_notation='A';
    int row_notation=1;
    print_table(table_width+1,table_height+1,column_notation,row_notation);

    //Αρχικοποίηση πίνακα

    for(int i = 0; i < table_height; i++)
    {
        for(int j = 0; j < table_width; j++)
        {
            Actual_Table[i][j] = 1.11111111111;
        }
    }

    GetConsoleMode(hin,&fdwSaveOldMode);
    SetConsoleMode(hin, fdwNewMode);

    while(exit_flag)
    {
        SetConsoleScreenBufferSize(hout,Screen_buffer_size); //https://docs.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
        ReadConsoleInput(hin, &InputRecord,1,&Events);
        if(counter <= 20)
            SetConsoleCursorPosition(hout,default_coord);
        //Event handler (της windows.h) που λαμβάνει είσοδο από τον χρήστη

        switch(InputRecord.EventType)                       //https://stackoverflow.com/questions/35797336/how-to-find-the-mouse-button-click-position-x-and-y
        {

         case KEY_EVENT: // Εισαγωγή από το πληκτρολόγιο

                    switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
                    {
                        //Εάν το πλήκτρο είναι το Control
                        case VK_CONTROL:
                            SetConsoleMode(hin,fdwSaveOldMode);
                            fgets(str,20,stdin);
                            fflush(stdin);

                            //Αρχή μιας αλυσίδας από if που ελέγχει το αλφαριθμητικό str και καλεί τις κατάλληλες
                            //συναρτήσεις, αναλόγως του input του χρήστη

                            if(strncmp(str,"help",4) == 0 )
                            {
                                Clear_previous_messages(default_coord,hout);
                                printf("\n\nHelp:\n-Type: \"add row\" or \"add column\" and specify the row or column to add\
 all its elements\n-Type: \"median row\" or \"median column\" and specify the row or column to calculate its median\n\
-Type: \"average row\" or \"average column\" and specify the row or column to calculate its average\n\
-Type: \"min-max column-row and specify the row or column to calculate its minimum and maximum value\n\
-Type: \"mult column-row\" and specify the row or column to multiply all its elements\n\
-Type: \"clear\" to clear the whole table\n\
-Type: \"exit\" to close the program" );
                            }
                            else if(strncmp(str,"median",6) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double m = median(c[0],k,table_height,table_width,Actual_Table);
                                print_result(k,c[0],m,hout,default_coord);
                            }
                            else if(strncmp(str,"add",3) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = add(c[0] , k , table_width , table_height , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"mult",4) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = multiply(c[0] , k , table_height , table_width , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"clear",5) == 0)
                            {
                                for(int i = 0; i < table_height; i++)
                                {
                                    for(int j = 0; j < table_width; j++)
                                    {
                                        Actual_Table[i][j] = 1.11111111111;
                                    }
                                }
                                visual_clear(default_coord,hout);
                            }
                            else if(strncmp(str,"average",7) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = average(c[0] , k , table_width , table_height , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"min",3)==0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double minim = min_value(c[0], k, table_height, table_width, Actual_Table);
                                print_result(k,c[0],minim,hout,default_coord);
                            }
                            else if(strncmp(str,"max",3)==0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double maxim = max_value(c[0], k, table_height, table_width, Actual_Table);
                                print_result(k,c[0],maxim,hout,default_coord);
                            }
                            else if(strncmp(str,"exit",4) == 0)
                            {
                                Clear_previous_messages(default_coord,hout);
                                exit_flag  = 0;
                            }

                            //Εδώ γίνεται ο έλεγχος για το εάν ο χρήστης προσπαθεί να προσθέσει τιμή στον
                            //πίνακα μέσω της γραμμής εντολών, π.χ. Α1: 32.4. Επίσης να σημειωθεί οτί
                            //παρομοίως με το Α1: 32.4 δουλεύει και το 1Α: 32.4

                            for(int i = 0; i < 12; i++)
                            {
                                if(str[i] == 58) //Ελέγχουμε εάν το αλφαριθμητικό έχει τον χαρακτήρα ':' μέσα.
                                {
                                    inserted_coord = Handle_input(str);

                                    //Ελέγχουμε εαν οι συντεταγμένες που έβαλε ο χρήστης είναι μέσα στα όρια του πίνακα
                                    //και ότι δεν υπήρξε κάποιο σφάλμα στη συνάρτηση Handle_input().

                                    if(inserted_coord.X != 100 && inserted_coord.Y != 100)
                                    {
                                        if(inserted_coord.Y < table_height && inserted_coord.X < table_width)
                                        {
                                            Add_num_to_array(&str[i+1],table_width,table_height, Actual_Table, inserted_coord, default_coord, hout);
                                        }
                                    }
                                }
                            }

                            //Βάζουμε τον κέρσορα πίσω στην προεπιλεγμένη του θέση (στη γραμμή εντολών)

                            SetConsoleCursorPosition(hout,default_coord);
                            //Καθαρίζουμε την γραμμή εντολών από τυχόν χαρακτήρες που έχουν απομείνει από προηγούμενο input.
                            for(int i=0;i<table_width*13;i++)
                                printf(" ");
                            SetConsoleCursorPosition(hout,default_coord);
                            SetConsoleMode(hin,fdwNewMode);
                            break;

                        //Debug κώδικας που τυπώνει τον πίνακα και τις τιμές του όταν πατηθεί το κουμπί SHIFT.

                        /*case VK_SHIFT:
                            printf("\n");
                            for(int i = 0; i < table_height; i++)
                            {
                                for(int j = 0; j < table_width; j++)
                                {
                                    printf("%14f",Actual_Table[i][j]);
                                }
                                printf("\n");
                            }
                            break;*/
                    }

        case MOUSE_EVENT:  //Input από το ποντίκι.

            //Εάν βρίσκεται μέσα στα όρια του πίνακα, καθώς και εάν ο χρήστης έχει πατήσει αριστερό κλικ.

             if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED &&
                InputRecord.Event.MouseEvent.dwMousePosition.Y < 5 + 2*table_height && InputRecord.Event.MouseEvent.dwMousePosition.X > 3 &&
                InputRecord.Event.MouseEvent.dwMousePosition.X < 3 + 13*table_width && InputRecord.Event.MouseEvent.dwMousePosition.Y%2 == 1 &&
                InputRecord.Event.MouseEvent.dwMousePosition.Y > 4)
                    {
                        coord.X = (((InputRecord.Event.MouseEvent.dwMousePosition.X-4)/13)*13 + 4); //κατάλληλοι υπολογισμοί για offset στην αρχή του κάθε κελιού και όχι στο σημείο όπου πατήθηκε.
                        coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                        SetConsoleCursorPosition(hout,coord);
                        printf("            ");                                                    //καθαρισμός του κελιού εικονικά από τυχόν προηγούμενα νούμερα
                        SetConsoleCursorPosition(hout,coord);
                        SetConsoleMode(hin, fdwSaveOldMode);
                        input(string,LENGTH);
                        coord.X = (coord.X-3)/13;
                        coord.Y = (coord.Y-5)/2;
                        Actual_Table[coord.Y][coord.X] = atof(string);                             //Καταχωρούμε την τιμή του χρήστη στο κατάλληλο σημείο μνήμης του πίνακα.
                        fflush(stdin);
                        SetConsoleCursorPosition(hout,default_coord);
                        SetConsoleMode(hin,fdwNewMode);
                    }
        }
        ++counter;
    }
}

//Συνάρτηση που κάνει πολλαπλασιαμό αριθμών στήλης και γραμμής, ανάλογα με την επιλογή του χρήστη.

/*Παίρνει το input string που έχει εισάγει ο χρήστης και είναι της μορφής column-row X, όπου Χ ο αριθμός
της στήλης-γραμμής του πίνακα στην οποία επιθυμεί να γίνει ο πολλαπλασιασμός. Ο ακέραιος k αντιπροσωπεύει την
τιμή X. Επίσης η συνάρτηση παίρνει ως όρισμα τον πίνακα και τις διαστάσεις του, για να μπορεί να αποκτήσει πρόσβαση
στις τιμές του με τις οποίες θα γίνει ο πολλαπλασιασμός. Εάν τα στοιχεία του δεν είναι το ουδέτερο στοιχείο μας 1.1111111111,
τότε οι τιμές του πολλαπλασιάζονται μεταξύ τους και επιστρέφεται το αποτέλεσμα τύπου double, αφού πρώτα ελεγχεί το str για
να διαπιστωθεί εάν η πράξη θα γίνει σε γραμμή ή στήλη*/
double multiply(char *str, int k, int table_height, int table_width , double table[table_height][table_width])
{
    double product = 1;
    int j = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    j++;
                    product *= table[i][k-1];
                }
            }
        if(j!=0)
            return product;
        else return 0;
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                j++;
                product *= table[k-1][i];
            }

        }
        if(j!=0)
            return product;
        else return 0;
    }
    else
    {
        return 0;
    }
}

/* "Καθαρίζει" εικονικά τον πίνακα από τις προηγούμενες τιμές του τυπώνοντας κενούς χαρακτήρες
σε κάθε κελί του. Παίρνει τις συντεταγμένες της γραμμής εντολής και το output handle του παραθύρου,
πληροφορίες που απαιτούνται και οι δύο για τη χρήση της συνάρτησης SetConsoleCursorPosition()*/
void visual_clear(COORD default_coord, HANDLE hout)
{
    COORD temp_coord;
    for(int i = 0; i < table_width+1; ++i)
    {
        for(int j = 0; j < table_height+1; ++j)
        {
            temp_coord.X = 4+i*13;
            temp_coord.Y = 5+2*j;
            SetConsoleCursorPosition(hout,temp_coord);
            printf("            ");
        }
    }
    SetConsoleCursorPosition(hout,default_coord);

}

/*Λαμβάνει το αλφαριθμητικό του χρήστη της μορφής "<εντολή> row/column <αριθμός γραμμής/στήλης>"
(π.χ. median row 1), τον αριθμό k που αντιστοιχεί στον αριθμό γραμμής/στήλης, καθώς και τους αριθμούς
των γραμμών και στηλών του πίνακα μνήμης, οι οποίοι αριθμοί χρησιμοποιούνται και για την μεταβίβαση
του ίδιου του πίνακα στην συνάρτηση, έτσι ώστε να έχει πρόσβαση στα στοιχεία του.
Η συνάρτηση επιλέγει μέσω if statements το αν θα εφαρμόσει τον αλγόριθμό της σε γραμμή ή στήλη
σύμφωνα με το αλφαριθμητικό του χρήστη που λαμβάνει. Δημιουργεί μετρητή cnt, τον οποίο χρησιμοποιεί
στην μέτρηση μέσω βρόχου επανάληψης των ουδέτερων στοιχείων 1.11111111111 που υπάρχουν στην γραμμή ή στήλη της οποίας την
διάμεσο θέλουμε να βρούμε. Στην συνέχεια όλα τα στοιχεία αυτής της γραμμής ή στήλης μεταβιβάζονται σε έναν
προσωρινό placeholder μονοδιάστατο πίνακα, που η συνάρτηση δημιουργεί ώστε να μπορεί να βάλει τα
στοιχεία σε διαφορετική σειρά χωρίς να επηρεάσει την γραμμή ή στήλη του πίνακα μνήμης. Μετά χρησιμοποιεί
βρόχους επανάληψης για να βάλει τα ουδέτερα στοιχεία στο τέλος του μονοδιάστατου πίνακα, και άλλους
βρόχους επανάληψης για να βάλει τα υπόλοιπα στοιχεία με άυξουσα σειρά. Μέσω if statements, επιλέγεται
ποιος αλγόριθμος θα ακολουθηθεί: αν ο αριθμός των μη-ουδέτερων στοιχείων είναι μονός, παίρνει και επιστρέφει το
μεσαίο από αυτά στοιχείο, ενώ αν είναι ζυγός, παίρνει και επιστρέφει τον μέσο όρο των δύο μεσαίων από αυτά στοιχεία.*/
double median(char *str, int k, int a, int b, double c[a][b])
{
    int cnt=0;
    double m,cmp;
    if(strncmp(str,"row",3) == 0)
    {
        for(int i=0;i<b;i++)
        {
            if(c[k-1][i]==1.11111111111)
            {
                cnt++;
            }
        }
        double temp[b];
        for(int i=0;i<b;i++)
        {
            temp[i]=c[k-1][i];
        }
        for(int j=0;j<b;j++)
        {
            for(int i=0;i<b;i++)
            {
                if(i!=b-1)
                {
                    if(temp[i]==1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        for(int j=0;j<b-cnt;j++)
        {
            for(int i=0;i<b-cnt;i++)
            {
                if(i!=b-cnt-1)
                {
                    if(temp[i]>temp[i+1] && temp[i+1]!=1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        if((b-cnt)%2!=0)
        {
            m=temp[(b-cnt-1)/2];
        }
        else
        {
            m=(temp[(b-cnt-1)/2]+temp[(b-cnt)/2])/2;
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
        for(int i=0;i<a;i++)
        {
            if(c[i][k-1]==1.11111111111)
            {
                cnt++;
            }
        }
        double temp[a];
        for(int i=0;i<a;i++)
        {
            temp[i]=c[i][k-1];
        }
        for(int j=0;j<a;j++)
        {
            for(int i=0;i<a;i++)
            {
                if(i!=a-1)
                {
                    if(temp[i]==1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        for(int j=0;j<a-cnt;j++)
        {
            for(int i=0;i<a-cnt;i++)
            {
                if(i!=a-cnt-1)
                {
                    if(temp[i]>temp[i+1] && temp[i+1]!=1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        if((a-cnt)%2!=0)
        {
            m=temp[(a-cnt-1)/2];
        }
        else
        {
            m=(temp[(a-cnt-1)/2]+temp[(a-cnt)/2])/2;
        }
    }
    return m;
}

//Συνάρτηση που κάνει πρόσθεση αριθμών στήλης και γραμμής, ανάλογα με την επιλογή του χρήστη.
double add(char *str, int k, int table_width, int table_height, double table[table_height][table_width])

{
    double sum = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    sum = sum + table[i][k-1];
                }
                //printf("%f\n",table[k-1][i]);
                //printf("\n true");
            }

        return sum;
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                sum = sum + table[k-1][i];
            }

        }
        return sum;
    }
    else
    {
        return 0;
    }
}

//Βρίσκει τον μέσο όρο των αριθμών στήλης και γραμμής, ανάλογα με την επιλογή του χρήστη.

/*Η συνάρτηση είναι η ίδια ακριβώς με την add(), με μόνη διαφορά τη μεταβλητή j. Αυτή, αυξάνεται κάθε φορά που
βρίσκουμε μια τιμή του πίνακα η οποία δεν είναι ίση με το ουδέτερο για εμάς στοιχείο 1.1111111111 και στο τέλος,
το αποτέλεσμα αποτελεί το άθροισμα όλων των τιμών αυτών διά τον αριθμό τους j.*/
double average(char *str, int k, int table_width, int table_height, double table[table_height][table_width])
{
    double sum = 0;
    int j = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    sum = sum + table[i][k-1];
                    j++;
                }
            }
        if(j !=0)
            return (sum/j);
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                sum = sum + table[k-1][i];
                j++;
            }

        }
        if(j != 0)
            return (sum/j);
    }
    return 0;
}

//Επιστρέφει τη μικρότερη τιμή που έχει βάλει ο χρήστης στην γραμμή-στήλη που αυτός επιλέγει (αν δεν έχει εισάγει αριθμούς επιστρέφει 0)
/*Παίρνει το input string που έχει εισάγει ο χρήστης και είναι της μορφής column-row X, όπου Χ ο αριθμός
της στήλης-γραμμής του πίνακα στην οποία επιθυμεί να γίνει ο πολλαπλασιασμός. Ο ακέραιος k αντιπροσωπεύει την
τιμή X. Επίσης η συνάρτηση παίρνει ως όρισμα τον πίνακα και τις διαστάσεις του, για να μπορεί να αποκτήσει πρόσβαση
στις τιμές που πρέπει να συγκρίνει για να βρεί την ελάχιστη. Αρχικά προσδιορίζεται εάν ο χρήστης επέλεξε η σύγκριση να γίνει
σε στήλη ή γραμμή του πίνακα και έπειτα η μεταβλητή min παίρνει την ελάχιστη τιμή με γνωστό τρόπο από τα παραδείγματα του κ. Σεβαστιάδη.
*/
double min_value(char *str,int k,int a, int b,double c[a][b])
{
    --k;
    double min;
    if(strncmp(str,"row",3) == 0)
    {
         for(int j = 0; j < table_width; j++)
        {
            if(c[k][j] != 1.11111111111)
            {
                min = c[k][j];
                break;
            }
            else min = 0;
        }
        for(int j = 0; j < table_width; j++)
        {
            if(min > c[k][j] && c[k][j] != 1.11111111111)
            {
                min = c[k][j];
            }
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
         for(int j = 0; j < table_width; j++)
        {
            if(c[j][k] != 1.11111111111)
            {
                min = c[j][k];
                break;
            }
            else min = 0;
        }
        for(int j = 0; j < table_height; j++)
        {
            if(min > c[j][k]&& c[j][k] != 1.11111111111)
            {
                min = c[j][k];
            }
        }
    }
    if(min != 1.11111111111)
        return min;
    else
        return 0;
}
//Επιστρέφει τη μέγιστη τιμή που έχει βάλει ο χρήστης στην γραμμή-στήλη που αυτός επιλέγει (αν δεν έχει εισάγει αριθμούς επιστρέφει 0
/*Παρόμοια με την min_value και λαμβάνοντας τα ίδια δεδομένα, η συνάρτηση max_value υπολογίζει τη μέγιστη τιμή της γραμμής ή στήλης που
ορίζει ο χρήστης και την επιστρέφει για να τυπωθεί. Η μεταβλητή max είναι αυτή που στο τέλος περιέχει τη μέγιστη τιμή και εάν ο χρήστης
δεν έχει εισάγει κανέναν αριθμό επιστρέφεται η τιμή 0.
*/
double max_value(char *str,int k,int a, int b,double c[a][b])
{
    --k;
    double max;
    if(strncmp(str,"row",3) == 0)
    {
        for(int j = 0; j < table_width; j++)
        {
            if(c[k][j] != 1.11111111111)
            {
                max = c[k][j];
                break;
            }
            else max = 0;
        }
        for(int j = 0; j < table_width; j++)
        {
            if(max < c[k][j] && c[k][j] != 1.11111111111)
            {
                max = c[k][j];
            }
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
        for(int j = 0; j < table_width; j++)
        {
            if(c[j][k] != 1.11111111111)
            {
                max = c[j][k];
                break;
            }
            else max = 0;
        }
        for(int j = 0; j < table_height; j++)
        {
            if(max < c[j][k] && c[j][k] != 1.11111111111)
            {
                max = c[j][k];
            }
        }
    }
    if(max != 1.11111111111)
        return max;
    else
        return 0;
}

/*Λαμβάνει το αποτέλεσμα μιας πράξης που θέλουμε να εκτυπωθεί, το αλφαριθμητικό της μορφής "<εντολή> row/column <αριθμός γραμμής/στήλης>"
που έχει εισάγει ο χρήστης (π.χ. add row 1), τον αριθμό k που αντιπροσωπεύει τον αριθμό γραμμής/στήλης, το output
handle του παραθύρου και τις προεπιλεγμένες συντεταγμένες (συντεταγμένες αρχής γραμμής εντολών).
Ορίζει δεδομένο τύπου COORD result_coord, το οποίο παίρνει τις κατάλληλες τιμές συντεταγμένων ανάλογα με
το αν πρόκειτα για πράξη στήλης ή γραμμής, σύμφωνα με if statements που χρησιμοποιούν το αλφαριθμητικό
του χρήστη, καθώς και με κατάλληλους υπολογισμούς σύμφωνα με τον αριθμό χαρακτήρων "-" (για οριζόντια)
και "|" (για κάθετα). Αφού βάλει τον κέρσορα στις σωστές συντεταγμένες, η συνάρτηση εκτυπώνει 12 χαρακτήρες
κενού ώστε να σβήσει οριακά οποιοδήποτε προηγούμενο αποτέλεσμα. Στη συνέχεια ξαναβάζει πίσω τον κέρσορα,
εκτυπώνει το αποτέλεσμα και τέλος βάζει τον κέρσορα στην προεπιλεγμένη θέση (αρχή γραμμής εντολών).*/
void print_result(int k,char *str, double res, HANDLE hout, COORD default_coord)
{
    COORD result_coord;
    if(strncmp(str,"row",3) == 0 && k<=table_height)
    {
        result_coord.X=4+13*table_width;
        result_coord.Y=3+2*k;
        SetConsoleCursorPosition(hout,result_coord);
        printf("            ");
        SetConsoleCursorPosition(hout,result_coord);
        printf("%g",res);
        SetConsoleCursorPosition(hout,default_coord);
    }
    else if(strncmp(str,"column",6) == 0 && k<=table_width)
    {
        result_coord.X=4+13*(k-1);
        result_coord.Y=5+2*table_height;
        SetConsoleCursorPosition(hout,result_coord);
        printf("            ");
        SetConsoleCursorPosition(hout,result_coord);
        printf("%g",res);
        SetConsoleCursorPosition(hout,default_coord);
    }

}

//Εισάγει το νούμερο επιλογής του χρήστη στον εικονικό πίνακα, καθώς και τον πίνακα μνήμης.
/*Λαμβάνει το αλφαριθμητικό που έχει εισάγει ο χρήστης και καλείται εάν αυτό περιέχει τον χαρακτήρα ':' μέσα του (Εάν είναι έγκυρο έχει τη μορφή ΧΧΧΧ,
όπου ΧΧΧΧ είναι ο αριθμός ο οποίος έχει εισαχθεί από τον χρήστη.). Επίσης, παίρνει τις διαστάσεις του πινάκα, τον ίδιο τον πίνακα,
 τις συντεταγμένες coord οι οποίες περιγράφουν σε ποιό μέρος της μνήμης [index] πρέπει να αποθηκευτεί ο αριθμός, τις συντεταγμένες
 της γραμμής εντολών και το output handle του παραθύρου. Αφού μετατρέψει τον αριθμό ΧΧΧΧ σε float, η συνάρτηση τον εισάγει στο κατάλληλο σημείο του πίνακα.
 Έπειτα, με μαθηματικές πράξεις προσδιορίζεται το σημείο στην οθόνη (το κελί του εικονικού πίνακα) όπου και μεταφέρεται ο κέρσορας για να γράψει την τιμή ΧΧΧΧ.
 Πρώτα όμως καθαρίζει το κελί τυπώνοντας κενούς χαρακτήρες, στην περίπτωση που υπήρχε ήδη μία τιμή μέσα. Ο καθαρισμός είναι εικονικός, ενώ αν υπήρχε ήδη παλιά
 τιμή στο σημείο μνήμης οπου θέλουμε να αποθηκεύσουμε την καινούρια, αυτή αντικαθίσταται απο την καινούρια. Τέλος, όταν έχει τυπωθεί ο χαρακτήρας, ο κέρσορας
 επιστρέφει στο σημείο της γραμμής εντολών και περιμένει κάποια δράση από τον χρήστη.*/
void Add_num_to_array(char *str, int table_width, int table_height, double table[table_height][table_width], COORD coord, COORD default_coord, HANDLE hout)
{
    double c = atof(str);
    //printf("%d,%d", coord.X,coord.Y);
    table[coord.Y][coord.X] = c;
    //printf("%d,%d", coord.X,coord.Y);
    coord.X = 4 + coord.X*13;
    coord.Y = 5 + coord.Y*2;
    SetConsoleCursorPosition(hout,coord);
    printf("            ");
    SetConsoleCursorPosition(hout,coord);
    printf("%g",c);
    SetConsoleCursorPosition(hout,default_coord);

}

//Αποκωδικοποιούνται οι κατάλληλες πληροφορίες από αλφαριθμητικά όπως πχ. Α3: ΧΧΧΧ ή 3Α: ΧΧΧΧ , όπου ΧΧΧΧ ο αριθμός που επιθυμεί ο χρήστης
//να εισάγει στις γραμμένες συντεταγμένες του πίνακα. Επιστρέφει COORD = {100,100} εάν υπάρχει κάποιο σφάλμα αναγνώρισης του αλφαριθμητικού
/*Η συνάρτηση λαμβάνει το string input του χρήστη ως μόνη παράμετρο. Αρχικά ελέγχουμε εάν ο πρώτος χαρακτήρας που έχει γραφεί είναι νούμερο
ή γράμμα μέσω του κώδικα ASCII. Εάν ο χαρακτήρας είναι γράμμα, ξέρουμε ότι ακολουθεί νούμερο. ρα θέτουμε την μεταβλητή user_coords έτσι ώστε
να πάρει τις κατάλληλες τιμές που αντιπροσωπεύουν τα index του πίνακα στα οποία πρέπει να αποθηκευτεί ο αριθμός. Αξιοσημείωτο είναι ότι η συνάρτηση
atoi() σταματά αυτόματα να διαβάζει input όταν βρεί έναν χαρακτήρα που δεν είναι αριθμός, άρα μπορούμε να διαβάσουμε και μονοψήφιους και διψήφιους αριθμούς
χωρίς κάποιο πρόβλημα. Εάν ο πρώτος χαρακτήρας είναι νούμερο, η μετατροπή είναι πιο δύσκολη. Πρέπει να βρούμε το index του πίνακα str από το οποίο θα ξεκινήσουμε
να διαβάζουμε για γράμματα το οποίο δεν είναι πάντα ίδιο επειδή ο χρήστης μπορεί να βάλει μονοψήφιο ή διψήφιο αριθμό. Με τη χρήση του while loop που έχουμε, η μεταβλητή
j παίρνει τιμή 1 ή 2 αντίστοιχα, ανάλογα με το αν ο αριθμός είναι μονοψήφιος ή διψήφιος και έτσι με τους κατάλληλους υπολογισμούς μπορούμε να αποκωδικοποιήσουμε το
μήνυμα του χρήστη*/
COORD Handle_input(char *str)
{
    COORD user_coords = {100,100};
    if(str[0] > 64 && str[0] < 91)
    {
        user_coords.X = str[0]-65;
        user_coords.Y = atoi(&str[1])-1;
    }
    else if(str[0] > 47 && str[0] < 58)
    {
        int j = 0;
        int temp = atoi(str)/pow(10,j);
        user_coords.Y = atoi(str)-1;
        while(temp != 0)
        {
            j++;
            temp = atoi(str)/pow(10,j);
        }
        user_coords.X = str[j] - 65;
    }
    else
    {
        user_coords.X = 100;
        user_coords.Y = 100;
        //printf("There was an error processing your request");
    }
    return user_coords;
}

//Τυπώνει τον γραφικό πίνακα με τον οποίο ο χρήστης μπορεί να αλληλεπιδράσει και να φανταστεί ευκολότερα τον πίνακα στη μνήμη.
/*Λαμβάνει το μήκος και ύψος του απεικονιζόμενου πίνακα, καθώς και δύο τύπους char για την σημειογραφία των γραμμών και στηλών.
Χρησιμοποιεί το γνωστό δεδομένο ότι το κάθε κελί έχει προκαθορισμένο μέγεθος δώδεκα χαρακτηρών οριζόντια και ενός χαρακτήρα κάθετα
για να εκτυπώσει με την χρήση βρόχων επανάληψης τα ανάλογα "-" και "|", σύμφωνα με τις διαστάσεις που εισάγει ο χρήστης. Τα δεδομένα
char αυξάνονται κατά 1 με την επανάληψη των βρόχων για να σημειογραφούν τις γραμμές και στήλες.*/
void print_table(int table_width,int table_height, char column_notation , char row_notation)
{
    printf("\n   |");
    for(int i=0;i<table_width;i++)
    {
        printf("     %c      |",column_notation++);
    }
    printf("\n");
    for(int j=0;j<table_height;j++)
    {
        for(int i=0;i<table_width;i++)
        {
            printf("-------------");
        }
        printf("----\n");
        if(row_notation<10)
            printf(" %d |",row_notation++);
        else
            printf("%d |",row_notation++);
        for(int i=0;i<table_width;i++)
        {
            printf("            |");
        }
        printf("\n");
    }
    for(int i=0;i<table_width;i++)
        {
            printf("-------------");
        }
    printf("----\n");
    printf("\n \nWelcome to \"Pepega's Computational Table\". You can press \"CONTROL\" to type your commands.\n\
For a list of all commands, please press \"CONTROL\" to begin typing, and then type \"help\".\nThank you for using \"Pepega's Computational Table\"");
}

//https://cboard.cprogramming.com/c-programming/14497-limiting-number-characters-typed.html
//Λήφθηκε από τον παραπάνω σύνδεσμο, μιας και στην C είναι δύσκολο να πετύχουμε αυτό που θα θέλαμε, δηλαδή να περιορίσουμε
//τον χρήστη έτσι ώστε να μήν μπορέσει να γράψει πάνω από συγκεκριμένο αριθμό χαρακτήρων. Παρ'όλο που τη χρησιμοποιούμε χωρίς
//εξηγήσεις, καταλαβαίνουμε πώς δουλεύει αλλά θα μας έπαιρνε υπερβολικά πολύ χρόνο να την εφηύρουμε μόνοι μας.
int input(char *string, int length)
{
   int  done  = FALSE;
   int  index = 0;
   char ch;

   string[0] = NUL;

   do
   {
      ch = getch();

      //check to see whether the buffer is full
      if(index == length)
      {
         switch(ch)
         {
            case CR:
               break;
            case BACKSPACE:
               break;
            default:
               ch = NUL;
         }
      }

      //process the keyboard input
      switch(ch)
      {
         case CR:
            putchar(ch);
            putchar(LF);
            string[index] = NUL;
            done = TRUE;
            break;
         case BACKSPACE:
            if(index == 0)
            {
               break;
            }
            else
            {
               putchar(ch);
               putchar(' ');
               putchar(ch);
               index--;
            }
            break;
         case TAB:
            break;
         case NUL:
            break;
         default:        //display & store
            putchar(ch);
            string[index] = ch;
            index++;
       }
   }
   while(!done);
}

//Καθαρίζει τα τυπωμένα αλφαριθμητικά και επιστρέφει τον κέρσορα πίσω στη γραμμή εντολών.
void Clear_previous_messages(COORD default_coord, HANDLE hout)
{
    default_coord.Y++;
    SetConsoleCursorPosition(hout,default_coord);
    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 150; j++ )
        {
                printf(" ");
        }
        printf("\n");
    }
    default_coord.Y--;
    SetConsoleCursorPosition(hout, default_coord);
}
