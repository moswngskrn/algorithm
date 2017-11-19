#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
using namespace std;


struct linkNode;

typedef struct Node {
    string name;
    struct linkNode* link=NULL;
} tNode;

typedef struct linkNode {
    string value;
    struct Node *toNode=NULL;
    struct linkNode* next=NULL;
} lNode;

//convert array to vector
vector<string> convArrToVect(string *A,int siz){
    vector<string> result;
    for(int i=0;i<siz;i++){
        result.push_back(A[i]);
    }
    return result;
}


//Input : vector string , ข้อความที่ต้องการหา
//Output : ตำแหน่งของ ค่า ที่อยู่ใน vector ถ้าไม่พบ return -1;
int posOfVectorString(vector<string> arr,string keyword){
    for(int i=0;i<arr.size();i++){
        if(arr[i]==keyword) return i;
    }
    return -1;
}

//มีค่าอยู่ใน vector
//Input : vector string , ข้อความที่ต้องการหา
//Output : true ,false
bool hasIn(vector<string> arr,string keyword){
    for(int i=0;i<arr.size();i++){
        if(arr[i]==keyword) return true;
    }
    return false;
}

//ทุกค่าใน vector 2d ชนิด string ในตำแหน่ง คอลลัมนั้น มีค่าเท่ากันทำหมด
//Input : vector 2d ชนิด string, ตำแหน่งคอลัม
//Output : true ,false
bool isAllOfColsEqual(vector< vector<string> > Array,int pos){
    for(int i=0;i<Array.size()-1;i++){
        if(Array[i][pos]!=Array[i+1][pos]) return false;
    }
    return true;
}


//ค้นหา target ที่มีมากที่สุด
//input : attributes เป็น ชื่อ ที่เก็บเป็นvector ชนิดstring,ข้อมูลที่ทดสอบ, target attribute
//Output : target ที่มากที่สุด
string majority(vector<string> attributes,vector< vector<string> > data,string target){

    //find target attribute
    map<string, int> valFreq;
    int index = posOfVectorString(attributes,target);
    for(int i=0;i<data.size();i++){
        if(valFreq[data[i][index]]!=0){
            valFreq[data[i][index]]++;
        }else{
            valFreq[data[i][index]] = 1;
        }
    }
    //find target in data
    int Max = 0;
    string major = "";
    for(map<string, int>::const_iterator iter = valFreq.begin(); iter != valFreq.end(); iter++){
        if(valFreq[iter->first]>Max){
            Max=valFreq[iter->first];
            major = iter->first;
        }
    }
    return major;
}

//คำนวน Entropy
double entropy(vector<string> attributes,vector< vector<string> > data,string targetAttr){
    map<string, double> valFreq;
    double dataEntropy = 0.0;
    //ค้นหาตำแหน่งของ targetAttr
    int indexOfTarget = posOfVectorString(attributes,targetAttr);
    //คำนวนความถี่
    for(int i=0;i<data.size();i++){
        if (valFreq[data[i][indexOfTarget]]!=0){
            valFreq[data[i][indexOfTarget]] += 1.0;
        }
        else{
            valFreq[data[i][indexOfTarget]]  = 1.0;
        }
    }
    //Calculate the entropy of the data for the target attr
    for(map<string, double>::const_iterator iter = valFreq.begin(); iter != valFreq.end(); iter++){
        dataEntropy += (-valFreq[iter->first]/data.size())*log2(valFreq[iter->first]/data.size());
    }
    return dataEntropy;
}


//gain
double gain(vector<string> attributes,vector< vector<string> > data,string attr,string targetAttr){
    //cout<<"entropy(parent) = "<<entropy(attributes, data, targetAttr)<<endl;
    //cout<<endl;
    map<string, double> valFreq;
    double subsetEntropy = 0.0;
    //ค้นหาตำหน่ง attribute ที่ต้องการหาค่า gain
    int indexOfAttr = posOfVectorString(attributes,attr);

    //ค้นหารว่า attribute ประกอบด้วยอะไรบ้างจำนวนเท่าไหร่
    for(int i=0;i<data.size();i++){
        if (valFreq[data[i][indexOfAttr]]!=0){
            valFreq[data[i][indexOfAttr]] += 1.0;
        }
        else{
            valFreq[data[i][indexOfAttr]]  = 1.0;
        }
    }
    //คำนวนหารค่าผลรวมของ entropy ของแต่ละ ค่า ของ attribute คูณกับ ความน่าจะเป็นของค่านั้น
    for(map<string, double>::const_iterator iter = valFreq.begin(); iter != valFreq.end(); iter++){
        double valProb = valFreq[iter->first] / data.size();
        //cout<<"freq("<<iter->first<<") = "<<valFreq[iter->first]<<endl;
        //cout<<"size = "<<data.size()<<endl;
        vector< vector<string> > dataSubset;
        for(int j=0;j<data.size();j++){
            if(data[j][indexOfAttr]==iter->first){
                dataSubset.push_back(data[j]);
            }
        }
        double entro = entropy(attributes, dataSubset, targetAttr);
        //cout<<"entropy("<<attr<<" = "<<iter->first<<") = "<<entro<<endl;
        //cout<<"p("<<attr<<" = "<<iter->first<<") = "<<valProb<<endl;
        //cout<<endl;
        subsetEntropy += valProb*entro;
    }
    //คำนวณหารค่า gain
    double ig = entropy(attributes, data, targetAttr) - subsetEntropy;
    //cout<<"IG(parent,"<<attr<<") = "<<ig<<endl;
    //cout<<"_____________________________________"<<endl;
    return ig;
}


//เลือก attibute
string chooseAttr(vector< vector<string> > data,vector<string> attributes,string target){
    string best = attributes[0];
    double maxGain = 0.0;
    double newGain;
    int posTarget = posOfVectorString(attributes,target);
    for(int i=0;i<attributes.size();i++){
        if(i!=posTarget){
            newGain = gain(attributes,data,attributes[i],target);
            if(newGain>maxGain && attributes[i]!=target){
                best = attributes[i];
                maxGain=newGain;
            }
        }
    }
    return best;
}


//ค้นหาว่า attribute ประกอบด้วยค่าอะไรบ้าง
vector<string> getValues(vector< vector<string> > data,vector<string> attributes,string attr){
    int index = posOfVectorString(attributes,attr);
    vector<string> values;

    for(int i=0;i<data.size();i++){
        if(!hasIn(values,data[i][index])){
            values.push_back(data[i][index]);
        }
    }
    return values;
}

//สร้าง Example ใหม่ ที่ตรงกับค่าroot(val)
vector< vector<string> > getExamples(vector< vector<string> > data,vector<string> attributes,string best,string val){
    vector< vector<string> > examples;
    int index = posOfVectorString(attributes,best);
    for(int i=0;i<data.size();i++){
        //ค้นหารายการที่ตรงกับ val
        if(data[i][index]==val){
            vector<string> newEntry;
            //เพิมค่าใน examples
            for(int j=0;j<data[i].size();j++){
                if(j!=index){
                    newEntry.push_back(data[i][j]);
                }
            }
            examples.push_back(newEntry);
        }
    }
    return examples;
}

//Examples คือตัวอย่างที่นำมาสอน
//Target_Attribute คือตัวแปรตาม
//Attribute คือตัวแปรต้น
//ID3 (Examples, Target_Attribute, Attributes)
map<string,map<string, string> > tree;
Node *ID3(vector< vector<string> > data,vector<string> attributes,string target){
    string deft = majority(attributes,data,target);
    int indexOfTarget = posOfVectorString(attributes,target);
    Node *root;
    if(isAllOfColsEqual(data,indexOfTarget)){
        root=new Node;
        root->name = data[0][indexOfTarget];
        return root;
    }
    else if(attributes.size()-1<=0){
        root=new Node;
        root->name = deft;
        return root;
    }
    else{
        string best = chooseAttr(data, attributes, target);
        vector<string> values = getValues(data, attributes, best);
        root = new Node;
        root->name=best;
        root->link = new linkNode;
        linkNode *ptr = root->link;
        for(int i=0;i<values.size();i++){
            vector< vector<string> > examples = getExamples(data, attributes, best, values[i]);
            vector<string> newAttr=attributes;
            newAttr.erase (newAttr.begin()+posOfVectorString(attributes,best));
            ptr->value=values[i];
            ptr->toNode = ID3(examples, newAttr, target);
            ptr->next = new linkNode;
            ptr=ptr->next;
        }
        return root;
    }
}


//--------- Size ----------
const int c = 5;
const int r = 14;

vector< vector<string> > readDataset(string data[][c],int row,int col){
    //cout<<"-------------read data set-------------"<<endl;
    vector< vector<string> > dataset;
    for(int i=0;i<row;i++){
        vector<string> attr;
        for(int j=0;j<col;j++){
            attr.push_back(data[i][j]);
        }
        dataset.push_back(attr);
    }
    return dataset;
}


linkNode * searchValue(linkNode *l,string val){
    if(l==NULL){
        return NULL;
    }
    if(l->value==val){
        return l;
    }
    return searchValue(l->next,val);
}
string testing(Node *modle,vector<string> attributes,vector<string> test){
    if(modle->link==NULL){
        //cout<<"("<<modle->name<<")"<<endl;
        return modle->name;
    }
    int index = posOfVectorString(attributes,modle->name);
    //cout<<"["<<modle->name<<" = ";
    linkNode *l1=searchValue(modle->link,test[index]);
    if(l1){
        //cout<<l1->value<<"] -> ";
        if(l1->toNode){
            return testing(l1->toNode,attributes,test);
        }
        else{
            return modle->name;
        }
    }else{
        return modle->name;
    }

}

int main()
{
    string dataA[r][c] = {{"sunny","hot","high","FALSE","no"},
                        {"sunny","hot","high","TRUE","no"},
                        {"overcast","hot","high","FALSE","yes"},
                        {"rainy","mild","high","FALSE","yes"},
                        {"rainy","cool","normal","FALSE","yes"},
                        {"rainy","cool","normal","TRUE	","no"},
                        {"overcast","cool","normal","TRUE","yes"},
                        {"sunny","mild","high","FALSE","no"},
                        {"sunny","mild","normal","FALSE","yes"},
                        {"rainy","mild","normal","FALSE","yes"},
                        {"sunny","mild","normal","TRUE","yes"},
                        {"overcast","mild","high","TRUE","yes"},
                        {"overcast","hot","normal","FALSE","yes"},
                        {"rainy","mild","high","TRUE","no"}};

    vector< vector<string> >  data = readDataset(dataA,r,c);
    string attb[c] = {"outlook","temperature","humidity","windy","play"};
    vector<string> attributes=convArrToVect(attb,c);
    string target = "play";
    Node *model = ID3(data,attributes,target);
    cout<<"===================================="<<endl;
    cout<<"=============== TEST ==============="<<endl;
    cout<<"===================================="<<endl;

    string test01[5] = {"overcast","hot","high","FALSE",""};//yes
    vector<string> test1= convArrToVect(test01,5);
    string test02[5] = {"rainy","cool","normal","FALSE",""};//yes
    vector<string> test2= convArrToVect(test02,5);
    string test03[5] = {"rainy","mild","normal","TRUE",""};//no
    vector<string> test3= convArrToVect(test03,5);
    string test04[5] = {"sunny","mild","high","FALSE",""};//no
    vector<string> test4= convArrToVect(test04,5);
    string test05[5] = {"sunny","mild","normal","FALSE",""};//yes
    vector<string> test5= convArrToVect(test05,5);

    cout<<"{\"overcast\",\"hot\",\"high\",\"FALSE\",\"\"} == yes : "<<(testing(model,attributes,test1)=="yes")<<endl;
    cout<<"{\"rainy\",\"cool\",\"normal\",\"FALSE\",\"\"} == yes : "<<(testing(model,attributes,test2)=="yes")<<endl;
    cout<<"{\"rainy\",\"mild\",\"normal\",\"FALSE\",\"\"} == yes : "<<(testing(model,attributes,test3)=="no")<<endl;
    cout<<"{\"sunny\",\"mild\",\"high\",\"TRUE\",\"\"} == yes : "<<(testing(model,attributes,test4)=="no")<<endl;
    cout<<"{\"sunny\",\"mild\",\"normal\",\"FALSE\",\"\"} == yes : "<<(testing(model,attributes,test5)=="yes")<<endl;
    return 0;
}
