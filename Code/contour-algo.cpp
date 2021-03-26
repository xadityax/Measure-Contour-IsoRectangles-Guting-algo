#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;


float bound_x_min = INT_MAX, bound_x_max = INT_MIN;

int counter = 0;
typedef struct cord{
    float x;
}cord;

typedef struct point{
    cord x;
    cord y;
}point;

typedef struct interval{
    cord bottom;
    cord top;
}interval;

typedef struct line_segment{
    interval inter;
    cord cordinate;
}line_segment;

typedef struct rectangle{
    cord x_left;
    cord x_right;
    cord y_bottom;
    cord y_top;
}rectangle;

typedef struct edge_type{
    int type; // 0,1,2,3 - left,right,bottom,top
}edge_type;

typedef struct edge{
    interval inter; // length
    cord cordinate; // x coord 
    edge_type e_type; // L R B T
}edge;

typedef struct lru{
    int val; //0,1,2 ==> left, right, undef
}lru;

typedef struct ctree{
    bool empty;
    float x;
    lru side;
    ctree *lson;
    ctree *rson;
    ctree(float a, int b){
        lson = NULL;
        rson = NULL;
        side.val = b;
        x = a;
        empty = false;
    }  
    ctree(bool x){
        empty = true;
        lson = NULL;
        rson = NULL;
        x = 777;
    }
}ctree;

typedef struct stripe_contour{
    interval x_inter;
    interval y_inter;
    ctree *tree; //vector<interval> x_union
    vector <interval> x_union;
}stripe_contour;

struct cmp{
    bool operator()(cord a, cord y){
        return a.x<y.x;
    }
};

struct cmp_partition{
    bool operator()(const cord &a, const cord &b){
        return a.x<b.x;
    }
};

bool cmp_stripe(stripe_contour a, stripe_contour b){
        if(a.y_inter.bottom.x<b.y_inter.bottom.x || (a.y_inter.bottom.x==b.y_inter.bottom.x && a.y_inter.top.x<b.y_inter.top.x ))
            return true;
        return false;
}

bool cmp_edge(edge a, edge b){
    if(a.inter.bottom.x<b.inter.bottom.x || (a.inter.bottom.x==b.inter.bottom.x && a.inter.top.x<b.inter.top.x))
        return true;
    return false;
}

bool stripe_subset(stripe_contour s1, stripe_contour s2){
    if((s1.y_inter.bottom.x >= s2.y_inter.bottom.x) && (s1.y_inter.top.x <= s2.y_inter.top.x)) 
        return true;
    return false; 
}

bool interval_subset(interval i1, interval i2){
    if((i1.bottom.x >= i2.bottom.x) && (i1.top.x <= i2.top.x)) 
        return true;
    return false; 
}

bool comparatorSort_yinter(stripe_contour S1, stripe_contour S2){
    if(S1.y_inter.bottom.x < S2.y_inter.bottom.x) return true;
    if(S1.y_inter.bottom.x == S2.y_inter.bottom.x && S1.y_inter.top.x < S2.y_inter.top.x) return true;
    return false;
}

bool comparatorSort_interval(interval i1, interval i2){
    if(i1.bottom.x < i2.bottom.x) return true;
    if(i1.bottom.x == i2.bottom.x && i1.top.x < i2.top.x) return true;
    return false;
}
void print_tree(ctree *tree){
    if(tree==NULL || tree->empty) return;
    print_tree(tree->lson);
    // cout << "Pushing " << tree->x << " given side type : " << tree->side.val << "\n";
    print_tree(tree->rson);
}


vector<cord> y_set(vector <rectangle> R){
    vector<cord> cord_set_vector;
    set <cord,cmp> cord_set;
    for (int i=0; i< R.size(); i++){
        cord c1, c2;
        c1.x= R[i].y_bottom.x;
        c2.x= R[i].y_top.x;
        cord_set.insert(c1);
        cord_set.insert(c2);
    }
    for(auto i: cord_set)
        cord_set_vector.push_back(i);
    return cord_set_vector;
}

vector<interval> partition(vector<cord> Y){  
    vector <interval> interval_set;
    sort(Y.begin(),Y.end(),cmp_partition());
    for(int i=0;i<Y.size()-1;i++){
        if(Y[i].x==Y[i+1].x)
            continue;
        interval in1;
        in1.bottom.x=Y[i].x;
        in1.top.x=Y[i+1].x;
        interval_set.push_back(in1);
    }
    return interval_set;
}

int bS1(vector<stripe_contour> S,interval test){
    int l = 0, r = S.size(),mid;
    if(r==0)
        return -1;
    while(l<=r){
        mid = l+(r-l)/2;
        if(S[mid].y_inter.bottom.x<=test.bottom.x && S[mid].y_inter.top.x>=test.top.x)
            return mid;
        else if(S[mid].y_inter.bottom.x<=test.bottom.x && S[mid].y_inter.top.x<test.top.x)
            l=mid+1;
        else r=mid-1;
    }
    if(S[mid].y_inter.bottom.x<=test.bottom.x && S[mid].y_inter.top.x>=test.top.x)
            return mid;
    return -1;
}

void copy(vector<stripe_contour> Sx1, vector<cord> &P, float leftx, float xm, vector<stripe_contour> &Sleft){
    // cout << "**********Copy Started************" << endl;
    interval Ix;
    Ix.bottom.x = leftx; Ix.top.x = xm;
    vector <interval> all_Iy = partition(P);
    for(auto intv:all_Iy){
        stripe_contour strp;
        strp.x_inter = Ix; 
        strp.y_inter = intv;
        strp.tree = new ctree(true);
        Sleft.push_back(strp);
    }
    vector<stripe_contour> stripe_to_ret;
    for(auto sdash:Sleft){
        int ind = bS1(Sx1,sdash.y_inter);
        if(ind!=-1){
            sdash.tree = Sx1[ind].tree;
        }
        // for(auto inner:Sx1){
        //     if(stripe_subset(sdash,inner)){
        //         sdash.tree = inner.tree;
        //         break;
        //     }
        // }
        stripe_to_ret.push_back(sdash);
    }
    Sleft.clear();
    Sleft = stripe_to_ret;
}

int bS2(vector<interval> S,interval test){
    // cout << "in bs2\n";
    int l = 0, r = S.size(),mid;
    if(r==0)
        return -1;
    while(l<=r){
        mid = l+(r-l)/2;
        if(S[mid].bottom.x<=test.bottom.x && S[mid].top.x>=test.top.x)
            return mid;
        else if(S[mid].bottom.x<=test.bottom.x && S[mid].top.x<test.top.x)
            l=mid+1;
        else r=mid-1;
    }
    if(S[mid].bottom.x<=test.bottom.x && S[mid].top.x>=test.top.x)
            return mid;
    return -1;
}

void blacken(vector<stripe_contour> &S, vector<interval> &J){
    if(J.size()==0)
        return;
    for(int i=0; i < S.size(); i++){
        int ind = bS2(J,S[i].y_inter);
        if(ind!=-1)
            S[i].tree = new ctree(true);
        // for(auto intv:J){
        //     if(interval_subset(S[i].y_inter,intv)){
        //         S[i].tree = new ctree(true);
        //         break;
        //     }
        // }
    }
}

struct cmpx{
    bool operator()(cord a, cord b){
        return a.x<b.x;    
    }
};

vector<cord> cords_union(vector<cord> A, vector<cord> B){
    set<cord,cmpx> un;
    vector <cord> ans;
    for(auto c:A)
        un.insert(c);
    for(auto c:B)
        un.insert(c);
    for(auto c:un)
        ans.push_back(c);
    return ans;
}
struct comp{
    bool operator()(const interval &i,const interval &j){ 
        if(i.bottom.x<j.bottom.x || (i.bottom.x==j.bottom.x && i.top.x<j.top.x))
            return true;
        return false;
    }
};

bool comp2_P(cord a,cord b){
    return a.x<b.x;
}

vector <interval> Intervals_union(vector<interval> arr1, vector<interval> arr2){ 
    if(arr1.size()==0)
        return arr2;
    if(arr2.size()==0)
        return arr1;
    vector <interval> arr;
    for(auto intr:arr1)
        arr.push_back(intr);
    for(auto intr:arr2)
        arr.push_back(intr);
    sort(arr.begin(),arr.end(),comp());
    int index = 0;  
    for(int i=1; i<arr.size(); i++) { 
        if (arr[index].top.x >= arr[i].bottom.x){  
            arr[index].top.x = max(arr[index].top.x, arr[i].top.x); 
            arr[index].bottom.x = min(arr[index].bottom.x, arr[i].bottom.x); 
        }
        else{
            index++;
            arr[index] = arr[i]; 
        } 
    } 
    vector<interval> ans; 
    for(int i = 0; i <= index; i++){
        interval newI;
        newI.bottom.x = arr[i].bottom.x;
        newI.top.x = arr[i].top.x;
        ans.push_back(newI); 
    } 
    return ans;
}

bool stripe_match(stripe_contour i1, stripe_contour i2){
    if( (i1.y_inter.bottom.x == i2.y_inter.bottom.x) && (i1.y_inter.top.x == i2.y_inter.top.x) ) 
        return true;
    return false; 
}

int bS3(vector<stripe_contour> S,interval test){
    // cout << "in bS3\n";
    int l = 0, r = S.size(),mid;
    if(r==0)
        return -1;
    while(l<=r){
        mid = l+(r-l)/2;
        if(S[mid].y_inter.bottom.x==test.bottom.x && S[mid].y_inter.top.x==test.top.x)
            return mid;
        else if(S[mid].y_inter.bottom.x<test.bottom.x)
            l=mid+1;
        else if(S[mid].y_inter.bottom.x>test.bottom.x)
            r=mid-1;
        else if(S[mid].y_inter.bottom.x==test.bottom.x && S[mid].y_inter.top.x<test.top.x)
            l=mid+1;
        else if(S[mid].y_inter.bottom.x==test.bottom.x && S[mid].y_inter.top.x>test.top.x)
            r=mid-1;
        else r=mid-1;
    }
    if(S[mid].y_inter.bottom.x==test.bottom.x && S[mid].y_inter.top.x==test.top.x)
            return mid;
    return -1;
}

vector<stripe_contour> concat(vector<stripe_contour> S1, vector<stripe_contour> S2, vector<cord> P, interval x_int){
    vector<stripe_contour> S;
    // cout<<"concat \n";
    sort(S1.begin(), S1.end(), comparatorSort_yinter);
    sort(S2.begin(), S2.end(), comparatorSort_yinter);
    vector<interval> Iy = partition(P);
    for(auto intv:Iy){
        stripe_contour temp;
        temp.x_inter = x_int;
        temp.y_inter = intv;
        temp.tree = new ctree(true);
        S.push_back(temp);
    }
    vector<stripe_contour> ans;
    for(auto strp:S){
        stripe_contour st1;
        stripe_contour st2;
        int ind1 = bS3(S1,strp.y_inter);
        if(ind1!=-1){
            st1 = S1[ind1];
        }
        int ind2 = bS3(S2,strp.y_inter);
        if(ind2!=-1){
            st2 = S2[ind2];
        }
        // for(auto strp1:S1){
        //     if(strp1.y_inter.top.x==strp.y_inter.top.x && strp1.y_inter.bottom.x==strp.y_inter.bottom.x){
        //         st1 = strp1;
        //         break;
        //     }
        // }
        // for(auto strp2:S2){
        //     if(strp2.y_inter.top.x==strp.y_inter.top.x && strp2.y_inter.bottom.x==strp.y_inter.bottom.x){
        //         st2 = strp2;
        //         break;
        //     }
        // }
        if(st1.tree->empty==false && st2.tree->empty==false && st1.tree!=st2.tree){
            strp.tree->x = st1.x_inter.top.x; 
            
            strp.tree->side.val = 2;
            strp.tree->lson = st1.tree;
            strp.tree->rson = st2.tree;
            strp.tree->empty=false;
        }
        else if(st1.tree->empty==false && st2.tree->empty==true){
            strp.tree = st1.tree;
            
        }
        else if(st1.tree->empty==true && st2.tree->empty==false){
            strp.tree = st2.tree;
            
        }
        else strp.tree = new ctree(true);
        ans.push_back(strp);
    }
    return ans;
}


vector <interval> Intervals_subtraction(vector<interval> arrA, vector<interval> arrB){
    if(arrB.size()==0)
        return arrA;
    // cout << "In subtraction\n";
    vector<interval> temp; //empty
    vector <interval> arr1 = Intervals_union(arrA,temp);
    vector <interval> arr2 = Intervals_union(arrB,temp);
    sort(arr1.begin(),arr1.end(),comp());
    sort(arr2.begin(),arr2.end(),comp());
    int i=0,j=0;
    vector <interval> ans;
    while(i<arr1.size() && j<arr2.size()){
        if(arr1[i].bottom.x == arr2[j].bottom.x){
            if(arr1[i].top.x<arr2[j].top.x){
                arr1[i].bottom.x = arr1[i].top.x;
                arr2[j].bottom.x = arr1[i].top.x;
                i++;    
            }
            else if(arr1[i].top.x>arr2[j].top.x){
                arr1[i].bottom.x = arr2[j].top.x;
                arr2[j].bottom.x = arr2[j].top.x;
                j++;    
            }
            else{
                i++;
                j++;
            }
        }
        else if(arr1[i].bottom.x < arr2[j].bottom.x){
            interval int1;
            int1.bottom.x = arr1[i].bottom.x;
            int1.top.x = min(arr2[j].bottom.x,arr1[i].top.x);
            // i++;
            if(arr1[i].top.x>arr2[j].bottom.x){
                arr1[i].bottom.x = min(arr1[i].top.x,arr2[j].bottom.x);
            }
            else{
                i++;
            }
            ans.push_back(int1);
        }
        else if(arr1[i].bottom.x > arr2[j].bottom.x){
            j++;
        }
    }
    if(j>=arr2.size()){
        while(i<arr1.size()){
            interval int1;
            int1.bottom.x = arr1[i].bottom.x;
            int1.top.x = arr1[i].top.x;
            i++;
            ans.push_back(int1);
        }
    }
    return ans;
}


vector <interval> Intervals_intersection(vector<interval> arr1, vector<interval> arr2) { 
    // cout << "In intersection\n";
    if(arr1.size()==0)
        return arr1;
    if(arr2.size()==0)
        return arr2;
    int i = 0, j = 0; 
    vector <interval> ans;
    int n = arr1.size(), m = arr2.size(); 
    while (i < n && j < m){ 
        int l = max(arr1[i].bottom.x, arr2[j].bottom.x);  
        int r = min(arr1[i].top.x, arr2[j].top.x); 
        if (l < r){
            interval newI;
            newI.bottom.x = l;
            newI.top.x = r;
            ans.push_back(newI); 
        }
        if(arr1[i].top.x < arr2[j].top.x) 
            i++; 
        else
            j++; 
    } 
    return ans;
} 

void display_tree(vector<stripe_contour> S){
    cout << "No .of stripes = " << S.size() << endl;
    for(auto strp:S){
        cout << "*****Stripe start***" << endl;
        cout << "Stripe Y interval : " << strp.y_inter.bottom.x << " " << strp.y_inter.top.x << " " << "\n";
        cout << "Stripe X interval : " << strp.x_inter.bottom.x << " " << strp.x_inter.top.x << " " << "\n";    
        print_tree(strp.tree);
        cout << "*****" << endl;
    }
}

vector<stripe_contour> answer_stripes;
void Stripes(vector<edge> V, interval x_ext, vector<interval> &L,vector<interval> &R,vector<stripe_contour> &S,vector<cord> &P){
    if(V.size()==0) return;
    vector<interval> Lx1,Lx2,Rx1,Rx2;
    vector<stripe_contour> Sx1,Sx2;
    vector<cord> Px1,Px2;
    counter++; 
    //cout << "Inside Stripe call no = " << counter << "\n";
    //cout << "Stripes, x_ext recieved as:["<<x_ext.bottom.x<<","<<x_ext.top.x<<"]"<<endl;
    interval x_ext_save;
    //cout << "V size = " << V.size() << "\n";
    if(V.size() == 1){
        //cout << "Stripe Base Case Entered" <<endl;
        if(V[0].e_type.type == 0)
            L.push_back(V[0].inter);
        else R.push_back(V[0].inter);
        cord c1,c2,c3,c4;
        c1.x = INT_MIN; c2.x = V[0].inter.bottom.x; c3.x = V[0].inter.top.x; c4.x = INT_MAX; 
        P.push_back(c1); P.push_back(c2); P.push_back(c3); P.push_back(c4);
        vector <interval> tmp_intrv = partition(P);
        // cout<<"Number of intervals returned: "<<tmp_intrv.size()<<endl;
        for(auto intrv:tmp_intrv){
            stripe_contour s1;
            s1.x_inter = x_ext;
            s1.y_inter = intrv; 
            s1.tree = new ctree(true);
            S.push_back(s1);
        }
        for(int i=0; i<S.size(); i++){
            if(S[i].y_inter.bottom.x == V[0].inter.bottom.x  && S[i].y_inter.top.x == V[0].inter.top.x ){
                if(V[0].e_type.type == 0){
                    S[i].tree = new ctree(V[0].cordinate.x, 0); // left
                    //cout << "********** Created a left node with value = " << V[0].cordinate.x << endl;
                    interval a;
                    a.bottom.x = V[0].cordinate.x;
                    a.top.x = x_ext.top.x;
                    S[i].x_union.push_back(a);
                }
                else{
                    S[i].tree = new ctree(V[0].cordinate.x, 1); // right
                    //cout << "********** Created a right node with value = " << V[0].cordinate.x << endl;
                    interval a;
                    a.bottom.x = x_ext.bottom.x;
                    a.top.x = V[0].cordinate.x;
                    S[i].x_union.push_back(a);
                }
            }
        }
        counter--;
        //cout << "Returning base case " << counter <<" \n";
    } 
    else{
        // Divide
        vector<edge> V1,V2;
        sort(V.begin(),V.end(),cmp_edge);
        float xm;
        if(V.size()%2==0)
            xm=((V[V.size()/2].cordinate.x)+ (V[(V.size()/2)-1].cordinate.x))/2;
        else xm=V[V.size()/2].cordinate.x;
        //xm = (x_ext.bottom.x+x_ext.top.x)/2;
        // cout << "Xm is : " << xm << "\n";
        // vector<interval> L1,L2,R1,R2;
        for(int i=0;i<V.size();i++){
            // if(V[i].cordinate.x<=xm && V1.size()<(V.size()/2))
            //     V1.push_back(V[i]);
            if(V[i].cordinate.x<=xm && V1.size()<(V.size()/2))
                V1.push_back(V[i]);
            else
                V2.push_back(V[i]);
        }
        // cout << "V1 size : " << V1.size() << " V2 size : " << V2.size() << "\n";
        // Conquer
        interval intt;
        cord c1; 
        c1.x = xm;
        intt.bottom = x_ext.bottom;
        intt.top.x = xm;
        Stripes(V1,intt,Lx1,Rx1,Sx1,Px1);
        // x_ext_save=x_ext;
        // cout<<"**********After returning from Left Half*************\n";
        // for(auto i: Sx1)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl;
        interval intt2;
        cord c2; 
        c2.x = xm;
        intt2.bottom.x = xm;
        intt2.top = x_ext.top;
        Stripes(V2,intt2,Lx2,Rx2,Sx2,Px2);
        // cout<<"**********After returning from Right Half*************\n";
        // for(auto i: Sx2)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl;
        
        // Merge 
        // cout << "Print L1 : \n";
        // for(auto intv:Lx1)
        //     // cout << intv.bottom.x << "," << intv.top.x << "\n";
        // // cout << "Print L2 : \n";
        // for(auto intv:Lx2)
        //     cout << intv.bottom.x << "," << intv.top.x << "\n";
        // // cout << "Print R1 : \n";
        // for(auto intv:Rx1)
        //     cout << intv.bottom.x << "," << intv.top.x << "\n";
        // // cout << "Print R2 : \n";
        // for(auto intv:Rx2)
        //     cout << intv.bottom.x << "," << intv.top.x << "\n";
        
        sort(Lx1.begin(),Lx1.end(),comp()); sort(Lx2.begin(),Lx2.end(),comp());
        sort(Rx1.begin(),Rx1.end(),comp()); sort(Rx2.begin(),Rx2.end(),comp());
        sort(Px2.begin(),Px2.end(),comp2_P); sort(Px1.begin(),Px1.end(),comp2_P);
        sort(Sx1.begin(),Sx1.end(),cmp_stripe); sort(Sx2.begin(),Sx2.end(),cmp_stripe);
        // cout << "Before Set operations inside stripes \n";
        vector<interval> LR = Intervals_intersection(Lx1,Rx2);
        //cout << "Print LR : \n";
        for(auto intv:LR)
            cout << intv.bottom.x << "," << intv.top.x << "\n";
        // cout << "LR Size: " << LR.size() <<endl;
        vector<interval> Lm = Intervals_subtraction(Lx1,LR);
        //cout << "Print L1-LR : \n";
        for(auto intv:Lm)
            cout << intv.bottom.x << "," << intv.top.x << "\n";
        // cout << "Lm Size: " << Lm.size() <<endl;
        L = Intervals_union(Lm,Lx2);
        // cout << "L Size: " << L.size() <<endl;
        // cout << "After intervals \n";
        Lm = Intervals_subtraction(Rx2,LR);
        //cout << "Print R2-LR : \n";
        for(auto intv:Lm)
            cout << intv.bottom.x << "," << intv.top.x << "\n";
        
        // cout << "R2-LR Size: " << Lm.size() <<endl;
        R = Intervals_union(Rx1,Lm);
        // cout << "R Size : " << R.size() <<endl;
        P = cords_union(Px1,Px2);
        // cout << "P Size : " << P.size() << endl;
        vector<stripe_contour> Sleft,Sright;
        

        //cout << "** MAKING Copy**" << endl;
        copy(Sx1,P,x_ext.bottom.x,xm,Sleft);
        // display_tree(Sleft);
        copy(Sx2,P,xm,x_ext.top.x,Sright);
        // display_tree(Sright);
        // cout << "\n";

        // cout<<"After copy (Sleft): \n";
        // for(auto i: Sleft)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl; 
        // cout<<"After copy (Sright): \n";
        // for(auto i: Sright)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl; 
        //blacken
        Lm = Intervals_subtraction(Rx2,LR);

        // cout << "** MAKING BLACKEN**" << endl;
        blacken(Sleft, Lm);
        // display_tree(Sleft);
        Lm = Intervals_subtraction(Lx1,LR);
        blacken(Sright, Lm);
        // display_tree(Sright);
        // cout << "\n";

        //cout<<"After blacken (Sleft): \n";
        // for(auto i: Sleft)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl;
        // cout<<"After blacken (Sright): \n";
        // for(auto i: Sright)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl;
        //concat
        vector<stripe_contour> S_ret;
        S.clear();

        // cout << "** MAKING CONCAT**" << endl;
        S = concat(Sleft,Sright,P,x_ext);
        // display_tree(S);
        cout << "\n";

        answer_stripes.clear();
        answer_stripes = S;
        // cout<<"After concat: \n";
        // for(auto i: S)
        //     cout<<"y_inter: ["<<i.y_inter.bottom.x<<", "<<i.y_inter.top.x<<"]"<<endl;
        // counter--;
        // cout << "Retuning from Stripes Case 2" << counter <<"..............................................." <<'\n';
        // answer in S
    }
}

bool cmp_last(edge a, edge b){
    if(a.cordinate.x<b.cordinate.x)
        return true;
    else if(a.cordinate.x==b.cordinate.x && a.e_type.type==0 && b.e_type.type==1){
        return true;
    }
    return false;
}

vector <stripe_contour> rectangle_dac(vector<rectangle> R,vector<edge> &VRX ){
    int xmin=INT_MAX,xmax=INT_MIN;
    //cout << "indac\n";
    for(int i=0;i < R.size(); i++){
        edge e1;
        e1.inter.bottom.x = R[i].y_bottom.x; e1.inter.top.x = R[i].y_top.x;
        e1.cordinate.x = R[i].x_left.x; e1.e_type.type = 0;
        edge e2;
        e2.inter.bottom.x = R[i].y_bottom.x; e2.inter.top.x = R[i].y_top.x;
        e2.cordinate.x = R[i].x_right.x; e2.e_type.type = 1;
        VRX.push_back(e1); VRX.push_back(e2);
    }
    sort(VRX.begin(),VRX.end(),cmp_last);
    interval base;
    base.bottom.x = INT_MIN;
    base.top.x = INT_MAX;
    vector<interval> L,Rt;
    vector<stripe_contour> S;
    vector<cord> P;
    Stripes(VRX, base, L, Rt, S, P); // check return type
    return S;
}

bool interval_sort_cmp (interval a, interval b){
    if(a.bottom.x<b.bottom.x || (a.bottom.x==b.bottom.x && a.top.x<b.top.x))
        return true;
    return false;
}

vector <float> ass;
vector <float> lefts;
vector <float> rights;
void union_intervals(ctree *tree){
    if(tree==NULL || tree->empty) 
        return;
    union_intervals(tree->lson);
    if(tree->lson==NULL && tree->rson==NULL){
        // cout << "This is a leaf :  ";
        // cout << "Pushing " << tree->x << " given side type : " << tree->side.val << "\n";
        if(tree->side.val==0)
            lefts.push_back(tree->x);
        if(tree->side.val==1)
            rights.push_back(tree->x);
    }
    union_intervals(tree->rson);
}

void intervals(edge h, stripe_contour s,vector <interval> &J){
    lefts.clear(); rights.clear(); ass.clear();
    union_intervals(s.tree);
    vector <interval> uintervals;
    // NEW
    for(int i=0;i<lefts.size();i++){
        // if(lefts[i]==rights[i]) continue;
        interval a;
        a.bottom.x = lefts[i];
        a.top.x = rights[i];
        uintervals.push_back(a);
    }
    vector<interval> tmp1; 
    tmp1.push_back(h.inter);
    vector<interval> int_interval = Intervals_intersection(tmp1,uintervals);
    J = Intervals_subtraction(tmp1,int_interval);
    //cout << " J size inside intervals : " << J.size() << "\n";
}

vector<line_segment> cpieces;
void contour_pieces(edge h, vector<stripe_contour> S){
    vector <interval> J;
    if(h.e_type.type==2){
        for(auto strp:S){
            if(strp.y_inter.top.x == h.cordinate.x){
                // cout << "Forming intervals for bottom edge with y interval : " << strp.y_inter.top.x << "\n";
                vector <interval> J;
                intervals(h,strp,J);
                for(auto intrv:J){
                    // cout << "*** Final contour piece with interval : " << intrv.bottom.x << "," << intrv.top.x << " " << h.cordinate.x << "\n";
                    line_segment LS;
                    LS.inter = intrv;
                    LS.cordinate = h.cordinate;
                    cpieces.push_back(LS);
                }
                break;
            }
        }
    }
    else{ 
        //return;
        for(auto strp:S){
            if(strp.y_inter.bottom.x  == h.cordinate.x){
                // cout << "Forming intervals for top edge with y interval : " << strp.y_inter.bottom.x << "\n";
                vector <interval> J;
                intervals(h,strp,J);
                for(auto intrv:J){
                    // cout << "*** Final contour piece with inverval : " << intrv.bottom.x << "," << intrv.top.x << " " << h.cordinate.x << "\n";
                    line_segment LS;
                    LS.inter = intrv;
                    LS.cordinate = h.cordinate;
                    cpieces.push_back(LS);
                }
                break;
            }
        }
    }
    //cout << "Contour Pieces End\n" ;
}

void create_horizontal_edge(vector<rectangle> R, vector<edge> &VRX_horizontal){
    // cout << "Creating Horizontal Edges" << endl;
    for(int i=0;i < R.size(); i++){
        edge e1;
        e1.inter.bottom.x = R[i].x_left.x; e1.inter.top.x = R[i].x_right.x;
        e1.cordinate.x = R[i].y_bottom.x; e1.e_type.type = 2;
        edge e2;
        e2.inter.bottom.x = R[i].x_left.x; e2.inter.top.x = R[i].x_right.x;
        e2.cordinate.x = R[i].y_top.x; e2.e_type.type = 3;
        VRX_horizontal.push_back(e1); VRX_horizontal.push_back(e2);
    }
}

bool rect_comp(rectangle a, rectangle b){
    if(a.x_left.x<b.x_left.x)
        return true;
    else if(a.x_left.x==b.x_left.x && a.x_right.x<b.x_right.x)
        return true;
    else if(a.y_bottom.x<b.y_bottom.x)
        return true;
    else if(a.y_bottom.x==b.y_bottom.x && a.y_top.x<b.y_top.x)
        return true;
    return false;
}

bool cmp_pieces(line_segment a, line_segment b){
    return a.cordinate.x<b.cordinate.x;
}
bool cmp_pieces2(line_segment a, line_segment b){
    if(a.inter.bottom.x<b.inter.bottom.x || (a.inter.bottom.x==b.inter.bottom.x && a.inter.top.x<b.inter.top.x))
        return true;
    return false;
}

bool checkinrect(vector<rectangle> R, float y1, float y2, float x){
    int count = 0;
    for(auto r:R){
        if(x>r.x_left.x && x<r.x_right.x && y1>=r.y_bottom.x && y2<=r.y_top.x)
            count++;
    }
    //cout << y1 << " " << y2 << " " << x << " " << count << "\n";
    if(count==1) return true;
    return false;
}

int main(){
    int n;
    cout << "Number of Rectangles?\n";
    cin >> n;
    cout << "Enter L(x) R(x) B(y) T(y)\n";
    vector <rectangle> R;
    vector <rectangle> Rh;
    float x1,x2,y1,y2;
    float t1=INT_MIN,t2=INT_MAX;
    ofstream fin;
    fin.open("inp.txt");
    for(int i=0;i<n;i++){
        cin >> x1 >> x2 >> y1 >> y2;
        fin<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
        bound_x_max = max(bound_x_max, x2);
        bound_x_min = min(bound_x_min, x1);
        rectangle newR;
        newR.x_left.x = x1;
        newR.x_right.x = x2;
        newR.y_bottom.x = y1;
        newR.y_top.x = y2;
        rectangle r2;
        r2.x_left.x = y1;
        r2.x_right.x = y2;
        r2.y_bottom.x = x1;
        r2.y_top.x = x2;
        R.push_back(newR);
        Rh.push_back(r2);
    }   
    //cout << "here\n";
    vector <edge> VRX;
    vector <stripe_contour> S = rectangle_dac(R, VRX);
    S = answer_stripes;
    // for(auto strp:S){
    //     // cout << strp.y_inter.top.x << " " << strp.y_inter.bottom.x << " " << "\n";
    //     // cout << strp.x_inter.bottom.x << " " << strp.x_inter.top.x << " " << "\n";   
    // }
    vector<edge> VRX_horizontal;
    create_horizontal_edge(R, VRX_horizontal);
    for(auto edg: VRX_horizontal)
        contour_pieces(edg,S);
    float ans = 0;
    cout << "\n\n Cpieces \n\n";
    int clength_top_bottom = 0, clength_left_right = 0;
    sort(cpieces.begin(),cpieces.end(),cmp_pieces);
    ofstream fout;
    fout.open("out_horz.txt");
    //cout << "\n\n New Pieces \n\n";
    vector<line_segment> new_pieces;
    for(int i=0; i<cpieces.size();i++){
        vector<interval> temp;
        temp.push_back(cpieces[i].inter);
        cord k = cpieces[i].cordinate;
        while(i<cpieces.size()-1 && cpieces[i].cordinate.x == cpieces[i+1].cordinate.x){
            temp.push_back(cpieces[i+1].inter);
            i++;
        }
        sort(temp.begin(), temp.end(), interval_sort_cmp);
        vector<interval> empty;
        empty.push_back(temp[0]);
        temp = Intervals_union(temp, empty);
        for(int j=0;j<temp.size();j++){
            line_segment a;
            a.inter = temp[j];
            a.cordinate = k;
            new_pieces.push_back(a);
        }
    }

    for(auto ls:new_pieces){
        cout << ls.inter.bottom.x << " " << ls.inter.top.x << " " << ls.cordinate.x << "\n";
        fout << ls.inter.bottom.x << " " << ls.inter.top.x << " " << ls.cordinate.x << "\n";
        ans += (ls.inter.top.x-ls.inter.bottom.x);
    }

    VRX.clear(); cpieces.clear();
    S.clear();
    S = rectangle_dac(Rh, VRX);
    S = answer_stripes;
    // for(auto strp:S){
    //     cout << strp.y_inter.top.x << " " << strp.y_inter.bottom.x << " " << "\n";
    //     cout << strp.x_inter.bottom.x << " " << strp.x_inter.top.x << " " << "\n";    
    // }
    VRX_horizontal.clear();
    create_horizontal_edge(Rh, VRX_horizontal);
    for(auto edg: VRX_horizontal)
        contour_pieces(edg,S);

    cout << "\n\n Cpieces \n\n";
    sort(cpieces.begin(),cpieces.end(),cmp_pieces);
    ofstream fout2;
    fout2.open("out_vert.txt");

    new_pieces.clear();
    for(int i=0; i<cpieces.size();i++){
        vector<interval> temp;
        temp.push_back(cpieces[i].inter);
        cord k = cpieces[i].cordinate;
        while(i<cpieces.size()-1 && cpieces[i].cordinate.x == cpieces[i+1].cordinate.x){
            temp.push_back(cpieces[i+1].inter);
            i++;
        }
        sort(temp.begin(), temp.end(), interval_sort_cmp);
        vector<interval> empty;
        empty.push_back(temp[0]);
        temp = Intervals_union(temp, empty);
        for(int j=0;j<temp.size();j++){
            line_segment a;
            a.inter = temp[j];
            a.cordinate = k;
            new_pieces.push_back(a);
        }
    }

    for(auto ls:new_pieces){
        cout << ls.inter.bottom.x << " " << ls.inter.top.x << " " << ls.cordinate.x << "\n";
        fout2 << ls.inter.bottom.x << " " << ls.inter.top.x << " " << ls.cordinate.x << "\n";
        ans += (ls.inter.top.x-ls.inter.bottom.x);
    }
    cout << "contour length : " << ans << "\n";
}