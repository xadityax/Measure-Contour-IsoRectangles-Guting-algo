#include<bits/stdc++.h>
using namespace std;
float bound_x_min = INT_MAX, bound_x_max = INT_MIN;

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
    edge_type e_type; // l R B T
}edge;

typedef struct stripe{
    interval x_inter;
    interval y_inter;
    float x_measure;
}stripe;


struct cmp{
    bool operator()(cord a, cord y){
        return a.x<y.x;
    }
};
vector<cord> y_set(vector <rectangle> R){
    // cout<<"y_set \n";
    vector<cord> cord_set_vector;
    set <cord,cmp> cord_set;
    for (int i=0; i< R.size(); i++){
        cord c1, c2;
        c1.x= R[i].y_bottom.x;
        c2.x= R[i].y_top.x;
        cord_set.insert(c1);
        cord_set.insert(c2);
    }
    for(auto i: cord_set){
        cord_set_vector.push_back(i);
    }
    return cord_set_vector;
}

struct cmp_partition{
    bool operator()(const cord &a, const cord &b){
        return a.x<b.x;
    }
};

vector<interval> partition(vector<cord> Y){  
    // cout<<"partition \n";
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

bool cmp_stripe(stripe a, stripe b){
        if(a.y_inter.bottom.x<b.y_inter.bottom.x || (a.y_inter.bottom.x==b.y_inter.bottom.x && a.y_inter.top.x<b.y_inter.top.x ))
            return true;
        return false;
}

// struct cmp_stripe2{
//     bool operator() (const stripe & a, const stripe & b){
//         if(a.y_inter.top.x<b.y_inter.top.x || (a.y_inter.top.x==b.y_inter.top.x && a.y_inter.bottom.x<b.y_inter.bottom.x ))
//              return true;
//          return false;
//     }
//     bool operator() (const stripe & left, int right){
//         return left.y_inter.top.x < right;
//     }
//     bool operator() (int left, const stripe & right){
//         return left < right.y_inter.top.x;
//     }
// };

bool stripe_subset(stripe s1, stripe s2){
    if((s1.y_inter.bottom.x >= s2.y_inter.bottom.x) && (s1.y_inter.top.x <= s2.y_inter.top.x)) 
        return true;
    return false; 
}
int bS1(vector<stripe> S,interval test){
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
void copy(vector<stripe> Sx1, vector<cord> &P,float bottom,float xm, vector<stripe> &Sleft){
    interval Ix;
    Ix.bottom.x = bottom; Ix.top.x = xm;
    vector <interval> all_Iy = partition(P);
    for(auto intv:all_Iy){
        stripe strp;
        strp.x_inter = Ix;  strp.y_inter = intv;
        strp.x_measure=0; Sleft.push_back(strp);
    }
    sort(Sleft.begin(),Sleft.end(),cmp_stripe); sort(Sx1.begin(),Sx1.end(),cmp_stripe);
    vector<stripe> stripe_to_ret;
    for(auto sdash: Sleft){
        stripe str1;
        str1.x_inter=sdash.x_inter;
        str1.y_inter=sdash.y_inter;
        str1.x_measure=0;
        int ind = bS1(Sx1,sdash.y_inter);
        if(ind!=-1){
            str1.x_measure = Sx1[ind].x_measure;
            sdash.x_measure = Sx1[ind].x_measure;
        }
        stripe_to_ret.push_back(str1);
    }
    Sleft.clear();
    Sleft=stripe_to_ret;
}

//Compartor to use for lower bound on stripes (for blacken)
bool interval_subset(interval i1, interval i2){

    if( (i1.bottom.x >= i2.bottom.x) && (i1.top.x <= i2.top.x) ) 
        return true;
    return false; 
}

//Comparator to use for intervals based on top and bottom of the interval
bool comparatorSort_interval(interval i1, interval i2)
{
    if(i1.bottom.x < i2.bottom.x) return true;
    if(i1.bottom.x == i2.bottom.x && i1.top.x < i2.top.x) return true;
    return false;
}

//Comparator to sort the stripes using y_inter
bool comparatorSort_yinter(stripe S1, stripe S2){

    if(S1.y_inter.bottom.x < S2.y_inter.bottom.x) return true;
    if(S1.y_inter.bottom.x == S2.y_inter.bottom.x && S1.y_inter.top.x < S2.y_inter.top.x) return true;
    return false;
}

int bS2(vector<interval> S,interval test){
    cout << "in bs2\n";
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

vector<stripe> blacken(vector<stripe> S, vector<interval> J){
    cout<<"blacken rcvd\n";
    cout<<"S:\n";
    for(auto i: S){
        cout<<"y_inter: ["<<i.y_inter.bottom.x<<"-"<<i.y_inter.top.x<<"]\n";
        cout<<"x_measure: "<<i.x_measure<<endl;
    }
    cout<<"J:\n";
    for(auto i: J)
        cout<<"y_inter: ["<<i.bottom.x<<"-"<<i.top.x<<"]\n";
    sort(J.begin(), J.end(), comparatorSort_interval);
    sort(S.begin(), S.end(), comparatorSort_yinter);
    for(int i=0; i < S.size(); i++){
        int ind = bS2(J,S[i].y_inter);
        if(ind!=-1){
            S[i].x_measure = S[i].x_inter.top.x - S[i].x_inter.bottom.x;
        }
    }
    cout<<"^^^^^^^^After Blacken^^^^^"<<endl;
    for(auto i : S){
        cout<<"y_inter: "<<"["<<i.y_inter.bottom.x<<","<<i.y_inter.top.x<<"]"<<endl;
        cout<<"x_measure: "<<i.x_measure<<endl;
    }
    return S;
}

int bS3(vector<stripe> S,interval test){
    cout << "in bS3\n";
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

vector<stripe> concat(vector<stripe> S1, vector<stripe> S2, vector<cord> P, interval x_int){   
    // cout<<"concat \n";
	sort(S1.begin(), S1.end(), comparatorSort_yinter);
    sort(S2.begin(), S2.end(), comparatorSort_yinter);
    vector<stripe> S;
    vector <interval> all_Iy = partition(P);
    for(auto intv:all_Iy){
        stripe strp;
        strp.x_inter = x_int;
        strp.y_inter = intv;
        S.push_back(strp);
    }
    vector<stripe> ret;
    for(auto strp:S){
        cout << "strp measure : " << strp.x_measure << "\n";
        stripe st1;
        stripe st2;
        int ind1 = bS3(S1,strp.y_inter);
        if(ind1!=-1){
            st1 = S1[ind1];
        }
        int ind2 = bS3(S2,strp.y_inter);
        if(ind2!=-1){
            st2 = S2[ind2];
        }
        strp.x_measure = st1.x_measure+st2.x_measure;
        cout << "strp measure : " << strp.x_measure << "\n";
        ret.push_back(strp);
    }
    return ret;
    
}

struct comp{
    bool operator()(const interval &i,const interval &j){ 
        if(i.bottom.x<j.bottom.x || (i.bottom.x==j.bottom.x && i.top.x<j.top.x))
            return true;
        return false;
    }
};

bool comp2_P(cord a,cord b){
    return a.x<=b.x;
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

vector <interval> Intervals_subtraction(vector<interval> arrA, vector<interval> arrB){
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
            if(arr1[i].top.x>arr2[j].bottom.x){
                arr1[i].bottom.x = min(arr1[i].top.x,arr2[j].bottom.x);
            }
            else i++;
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
    int i = 0, j = 0; 
    vector <interval> ans;
    int n = arr1.size(), m = arr2.size(); 
    while (i < n && j < m) { 
        float l = max(arr1[i].bottom.x, arr2[j].bottom.x);  
        float r = min(arr1[i].top.x, arr2[j].top.x); 
        if (l <= r) {
            interval newI;
            newI.bottom.x = l;
            newI.top.x = r;
            ans.push_back(newI); 
        }
        if (arr1[i].top.x < arr2[j].top.x) 
            i++; 
        else
            j++; 
    } 
    return ans;
} 

vector<cord> cords_union(vector<cord> P1, vector<cord>P2){
    vector<cord> union_vector;
    set<cord, cmp>union_set;
    for(auto i: P1)
        union_set.insert(i);
    for (auto i: P2)
        union_set.insert(i);
    for(auto i: union_set)
        union_vector.push_back(i);
    return union_vector;
}

void Stripes(vector<edge> V, interval x_ext, vector<interval> &L,
    vector<interval> &R,vector<stripe> &S,vector<cord> &P){ 
    interval x_ext_save;
    cout<<"Stripes, x_ext recieved as:["<<x_ext.bottom.x<<","<<x_ext.top.x<<"]"<<endl;
    if(V.size()==0)
        return;
    cout << "V SIZE : " << V.size() << "\n";
    vector<interval> Lx1,Lx2;
    vector<interval> Rx1,Rx2;
    vector<stripe> Sx1,Sx2;
    vector<cord> Px1,Px2;
    if(V.size() == 1){   
        cout<<"Stripe Base case \n";
        if(V[0].e_type.type == 0)
            L.push_back(V[0].inter);
        else
            R.push_back(V[0].inter);

        cord c1,c2,c3,c4;
        c1.x = INT_MIN; c2.x = V[0].inter.bottom.x; c3.x = V[0].inter.top.x; c4.x = INT_MAX; 
        P.push_back(c1); P.push_back(c2); P.push_back(c3); P.push_back(c4);
        vector <interval> all_Iy = partition(P);
        cout<<"Number of intervals returned: "<<all_Iy.size()<<endl;
    	for(auto intv:all_Iy){
        	stripe strp;
        	strp.x_inter = x_ext;
        	strp.y_inter = intv;
            strp.x_measure=0;
        	S.push_back(strp);
    	}
        for(int i=0; i< S.size(); i++){
            if(S[i].y_inter.bottom.x == V[0].inter.bottom.x  && S[i].y_inter.top.x == V[0].inter.top.x){
                if(V[0].e_type.type == 0){
                  interval int1;
                  int1.bottom = V[0].cordinate;
                  int1.top = x_ext.top;
                  S[i].x_measure = x_ext.top.x - V[0].cordinate.x;
                }
                else{
                  interval int1;
                  int1.bottom = x_ext.bottom;
                  int1.top = V[0].cordinate;
                  S[i].x_measure = V[0].cordinate.x - x_ext.bottom.x;
                }
            }
        }
        cout<<"Returning base case \n";
    }
    else{
        // Divide
        vector<edge> V1,V2;
        float xm;
        if(V.size()%2==0)
            xm=((V[V.size()/2].cordinate.x)+ (V[(V.size()/2)-1].cordinate.x))/2;
        else xm=V[V.size()/2].cordinate.x;
        // vector<interval> L1,L2,R1,R2;
        for(int i=0;i<V.size();i++){
            if(V[i].cordinate.x<=xm && V1.size()<(V.size()/2))
                V1.push_back(V[i]);
            else
                V2.push_back(V[i]);
        }
        // Conquer
        interval intt;
        cord c1;
        c1.x = xm;
        intt.bottom = x_ext.bottom;
        intt.top = c1;
        x_ext_save=x_ext;
        Stripes(V1,intt,Lx1,Rx1,Sx1,Px1);
        x_ext=x_ext_save;
        interval intt2;
        cord c2;
        c2.x = xm;
        intt2.top = x_ext.top;
        intt2.bottom = c2;
        Stripes(V2,intt2,Lx2,Rx2,Sx2,Px2);
        sort(Lx1.begin(),Lx1.end(),comp()); sort(Lx2.begin(),Lx2.end(),comp()); sort(Rx1.begin(),Rx1.end(),comp()); sort(Rx2.begin(),Rx2.end(),comp());
        sort(Px2.begin(),Px2.end(),comp2_P); sort(Px1.begin(),Px1.end(),comp2_P);
        vector<interval> LR = Intervals_intersection(Lx1,Rx2);
        vector<interval> Lm = Intervals_subtraction(Lx1,LR);
        L = Intervals_union(Lm,Lx2);
        Lm = Intervals_subtraction(Rx2,LR);
        R = Intervals_union(Rx1,Lm);
        P = cords_union(Px1,Px2);
        vector<stripe> Sleft,Sright;
        copy(Sx1,P,x_ext.bottom.x,xm,Sleft);
        copy(Sx2,P,xm,x_ext.top.x,Sright);
        //blacken
        Lm = Intervals_subtraction(Rx2,LR);
        Sleft = blacken(Sleft, Lm);
        Lm = Intervals_subtraction(Lx1,LR);
        Sright = blacken(Sright, Lm);
        //concat results
        S.clear();
        S = concat(Sleft, Sright, P, x_ext);
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

vector<stripe> rectangle_dac(vector<rectangle> R){
    cout << "indac\n";
    vector<edge> VRX;
    for(int i=0;i < R.size(); i++){
        cout << "her3\n";
        edge e1;
        e1.inter.bottom.x = R[i].y_bottom.x;
        e1.inter.top.x = R[i].y_top.x;
        e1.cordinate.x = R[i].x_left.x;
        e1.e_type.type = 0;
        edge e2;
        e2.inter.bottom.x = R[i].y_bottom.x;
        e2.inter.top.x = R[i].y_top.x;
        e2.cordinate.x = R[i].x_right.x;
        e2.e_type.type = 1;
        VRX.push_back(e1);
        VRX.push_back(e2);
    }
    getchar();
    cout << "her2\n";
    sort(VRX.begin(),VRX.end(),cmp_last);
    cout << "her2\n";
    interval base;
    base.bottom.x = bound_x_min;
    base.top.x = bound_x_max;
    vector<interval> L,Rt;
    vector<stripe> S;
    vector<cord> P;
    Stripes(VRX, base, L, Rt, S, P); // check return type
    return S;
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
    else return false;
}

int main(){
    int n;
    cout << "Number of Rectangles?\n";
    cin >> n;
    cout << "Enter (x1, x2); (y1, y2)\n";
    vector <rectangle> R;
    float x1,x2,y1,y2;
    for(int i=0;i<n;i++){
        cin >> x1 >> x2 >> y1 >> y2;
        bound_x_max = max(bound_x_max, x2);
        bound_x_min = min(bound_x_min, x1);
        rectangle newR;
        newR.x_left.x = x1;
        newR.x_right.x = x2;
        newR.y_bottom.x = y1;
        newR.y_top.x = y2;
        R.push_back(newR);
    }
    //sort(R.begin(),R.end(),rect_comp);
    cout << "her\n";
    vector <stripe> S = rectangle_dac(R);
    cout<<"Stripes Returned by rectangle_dac:\n";
    for(auto i: S){
         cout<<"y_inter: "<<"["<<i.y_inter.bottom.x<<","<<i.y_inter.top.x<<"]"<<" and ";
         cout<<"x_measure: "<<i.x_measure<<endl;
    }
    float area=0;
    for(auto i: S)
        area += i.x_measure*(i.y_inter.top.x-i.y_inter.bottom.x);
    cout << "so, FINALLY, Measure is: "<<area<<endl;
}