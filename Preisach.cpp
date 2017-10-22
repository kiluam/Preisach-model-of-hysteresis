#include<iostream>
#include<vector>
#include<string>
using namespace std;

//Function Definitions
void inputU(vector<double>&, bool&, double& );
//inputU - Take inputs u(t)
//@param vector<double>& - reference of the input vector
//@param bool& - reference to the system state

int main()
{
    vector<double> preisachVec;
    bool system = false;             // system state. Initially the system state was Off and hence declared false.
    double threshold = -10;         // system threshold assigned with -10.
    cout<<"The system threshold is " << threshold <<endl;
    inputU(preisachVec,system,threshold);
    //cout<<preisachVec.end()<<endl;
//    cout<<"-------"<<endl;
//    cout<<"The system output is: "<<endl;
//    preisachVec.erase(preisachVec.end()-2,preisachVec.end());  // Removes the last two similar elements.
//    for (unsigned int i=0; i<preisachVec.size(); i++)
//    {
//        cout<<preisachVec[i]<< " "<<flush;
//    }
    return 0;
}
//Function Declaration
void inputU(vector<double>& preisachVec, bool& systemStat, double& threshold )
{
    double betaI = 0;                        //indicator for repetition of beta.When 1, indicates there is repetition,else 0.
    double alphaI = 0;                       //indicator for repetition of alpha.When 1, indicates there is repetition,else 0.
    double userIp;                         //variable for user input.
    int dir;                                //variable that stores the previous direction.When 1,denotes UP-State, 0 otherwise.
    double alpha_min;                       //global minimum value of alpha.
    double beta_max = 0;                      //global maximum value of beta.
    do                                      //Allows user to input repeatedly,till condition (*) is satisfied.
    {
        cout<<"Please enter the system input: "<<flush;
        cin>>userIp;                        //user input for present system input
        if (userIp>threshold)
        {
            systemStat=true;                //system switches-up once greater than threshold
            preisachVec.push_back(userIp);
            dir = 1;   //dir=UP;                       //direction switching-up, once the system starts
            beta_max = userIp;
            alpha_min = beta_max;
        }
        else
        {
            cout<<"The input was less than the threshold and hence it is still off. "<<endl; //prompt the user to input again.
        }
    }
    while(systemStat == false);               //allows user to input, until the system starts (*)
    do
    {
        cout<<"Please enter the system input: "<<flush;   //while is system state is ON , the user is allowed for the next system input.
        cin>>userIp;
        if(userIp >= beta_max)                     //When the present input exceeds the previous maximum value of beta.
        {
            dir = 1;                              //direction changed to Up
            preisachVec.erase(preisachVec.begin(), preisachVec.end()); //wiping out the previous states
            preisachVec.push_back(userIp);
            beta_max=userIp;                    //beta maximum is assigned with the present input.
            alpha_min=beta_max;                 //alpha minimum is assigned with the present input.
        }
        else if(userIp <= alpha_min)
        {
            dir = -1;                             //direction changed to down
            preisachVec.erase(preisachVec.begin(),preisachVec.end()); //wiping out the previous states
            preisachVec.push_back(beta_max);
            preisachVec.push_back(userIp);
            alpha_min=userIp;
        }
        else
        {
            if (dir == -1 && userIp >= preisachVec.back())    //switching from down state to up state
            {
                for (unsigned int i=0; i<preisachVec.size(); i+=2)  //checking whether there is any previous UP state
                {
                    if (userIp > preisachVec[i] )
                    {
                        dir=1;
                        preisachVec.erase(preisachVec.begin()+i,preisachVec.end()); //wiping out
                        preisachVec.push_back(userIp);                      // inserting present value
                    }
                }
                if (preisachVec.back() != userIp  )    //if the present up state value less than earlier up state
                {                                      // values, insert this.
                    dir=1;
                    preisachVec.push_back(userIp);
                }
            }
            else if (dir == 1 && userIp <= preisachVec.back())    //switching from up state to down state
            {
                for (unsigned int i = 1; i<preisachVec.size(); i+=2)  //checking whether there is any previous down state
                {                                                   //value greater than the present value.
                    if (userIp<preisachVec[i])
                    {
                        dir=-1;
                        preisachVec.erase(preisachVec.begin()+i,preisachVec.end());//wiping out
                        preisachVec.push_back(userIp);      // inserting present value
                    }
                }
                if (preisachVec.back() != userIp)   //if the present up state value less than earlier up state
                {                                   // values, insert this.
                    dir=-1;
                    preisachVec.push_back(userIp);
                }
            }
            else if (dir == 1 && userIp > preisachVec.back()) //To check the case when previous state was in up state
            {                                               //and still the system remains in up state, and the present value is greater than the nearest last up state value.
                preisachVec.pop_back();              //Then remove the last up state value.
                preisachVec.push_back(userIp);      //Insert the present value.
            }
            else                        //To check the case when previous state was in down state
            {                           //and still the system remains in down state, and the present value is less than the nearest last down state value.
                preisachVec.pop_back(); //Then remove the last down state value.
                preisachVec.push_back(userIp);  //Insert the present value.
            }
        }
        if(preisachVec.size() >= 4 && preisachVec.size()%2 == 0)    // This condition deals with the stopping criterion. This happens when the Up state and down state value repeat.
        {
            for (unsigned int i = 0; i<preisachVec.size(); i+=2)
            {
                for (unsigned int j = i+2; j<preisachVec.size(); j+=2)
                {
                    if (preisachVec[i] == preisachVec[j])
                    {
                        betaI=1;
                    }
                    if (preisachVec[i+1] == preisachVec[j+1])
                    {
                        alphaI=1;
                    }
                }
            }
        }
        cout<<"The present system output is: "<<endl;
        for (unsigned int i = 0; i<preisachVec.size(); i++) //Prints the system output
        {
            cout<<preisachVec[i]<< " "<<flush;
        }
        cout<<""<<endl;
    }
    while(betaI != 1 || alphaI != 1);
}
