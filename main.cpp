#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <map>

int getseed() {
	std::ifstream in("/dev/random");char c;int num = 0;for (int i = 0; i < 5; i++) {in >> c;	num += num * 34567 + c;}
	return num;
}

#define st first
#define nd second
#define mp make_pair
#define pb push_back
#define dbg(x) cerr << #x << ": " << x << endl

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int inf = 1e9;

vector< pair<int, vector<int>> > hor, ver;
int n;
int rt[100005], loc[100005];
ll total;
map<string, int> tag2id;
	vector< pii > show;


vector<int> merge(const vector<int> &v1, const vector<int> &v2) {
	vector<int> res;

	int i = 0, j = 0;

	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j]) {
			if (res.size() == 0 || res[res.size()-1] != v1[i])
				res.pb(v1[i]); 
			i++;
		}
		else {
			if (res.size() == 0 || res[res.size()-1] != v2[j])
				res.pb(v2[j]); 
			j++;
		}
	}

	while(i < v1.size()) { res.pb(v1[i]); i++; }
	while(j < v2.size()) { res.pb(v2[j]); j++; }

	return res;
}

void printvec(const vector<int> v) {
	for (auto x: v) cout << x << " ";
	cout << endl;
}

int score(const vector<int> &v1, const vector<int> &v2) {
	// printvec(v1);
	// printvec(v2);

	int le = 0, mi = 0, ri = 0, j = 0;

	for (int i = 0; i < v1.size(); i++) {
		bool flag = true;

		while(j < v2.size() && v2[j] < v1[i]) {
			ri++;
			j++;
		}

		while(j < v2.size() && v2[j] == v1[i]) {
			mi++;
			j++;

			flag = false;
		}

		le += flag;
	}

	ri += v2.size() - j;

	 // cout << le << " " << mi << " " << ri << endl;

	return min(le, min(mi, ri));
}

inline vector<int> get_vec(pii p1) {
	if (p1.nd == -1) {
		return hor[p1.st].nd;
	}
	else {
		return merge(ver[p1.st].nd, ver[p1.nd].nd);
	}
}

int score(pii p1, pii p2) {
	return score(get_vec(p1), get_vec(p2));
}

void save(char *file) {
	ofstream save(file);

	for (auto x: hor) {
		save << x.st << " " << x.nd.size() << endl;
		for (auto y: x.nd) {
			save << y << " ";
		}
		save << endl;
	}

	for (auto x: ver) {
		save << x.st << " " << x.nd.size() << endl;
		for (auto y: x.nd) {
			save << y << " ";
		}
		save << endl;
	}

	save << show.size() << endl;
	for (auto &x: show) {
		save << x.st << " " << x.nd << endl;
	}

	save << total << endl;
}

void load(char *file) {
	ifstream save(file);
	int t, t2;

	for (auto &x: hor) {
		save >> x.st >> t;
		x.nd.resize(t);
		for (auto &y: x.nd) {
			save >> y;
		}
	}

	for (auto &x: ver) {
		save >> x.st >> t;
		x.nd.resize(t);
		for (auto &y: x.nd) {
			save >> y;
		}
	}

	save >> t;
	show.resize(t);
	for (auto &x: show) {
		save >> x.st >> x.nd;
	}

	save >> total;
}



int main(int argc, char **argv)
{
	char rot;
	int ntags;
	char buf[1000];

	srand(getseed() + time(0));

	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf(" %c %d", &rot, &ntags);

		vector<int> tags;

		while (ntags--) {
			scanf(" %s", buf);
			string tag(buf);

			if (tag2id.find(tag) == tag2id.end()) {
				int id = tag2id.size() + 1;
				tag2id[tag] = id;
			}

			tags.pb(tag2id[tag]);
		}

		sort(tags.begin(), tags.end());
		unique(tags.begin(), tags.end());

		if (rot == 'H') {
			loc[i] = hor.size();
			rt[i] = 0;
			hor.pb(mp(i, tags));
		}
		else {
			loc[i] = ver.size();
			rt[i] = 1;
			ver.pb(mp(i, tags));
		}
	}
	tag2id.clear();

	if (argc > 3) {
		cerr << "loading from ouput" << endl;
		int t;
		ifstream o(argv[3]);

		o >> t;
		show.resize(t);
		for (auto &x: show) {
			o >> x.st;
			x.nd = -1;
			x.st = loc[x.st];

			if (rt[x.st] == 1) {
				o >> x.nd;
				x.nd = loc[x.nd];
			}
		}

		for (int i = 1; i < show.size(); i++) {
			total += score(show[i-1], show[i]);
		}

		cerr << "done" << endl;
	}
	else if (argc > 2) {
		cerr << "loading" << endl;
		load(argv[1]);
	}
	else {
		random_shuffle(hor.begin(), hor.end());
		random_shuffle(ver.begin(), ver.end());
	
	

		int ih = 0, iv = 0;


		while(ih < hor.size() || iv+1 < ver.size()) {
			bool choice = ((ll)rand()*rand()) % (ver.size() / 2 + hor.size()) < hor.size();

			if (ih == hor.size())
				choice = 0;
			if (iv >= ver.size()-1)
				choice = 1;

			pii slide;

			if (choice) {
				// horizontal
				
				slide = mp(ih, -1);
				ih++;
			}
			else {
				// vertical

				slide = mp(iv, iv+1);
				iv += 2;
			}

			if (!show.empty()) {
				total += score(show.back(), slide);
			}

			show.pb(slide);
		}
	}



	// optimize

	int MAX = 50000, last = 0; 
	for (int i = 0; i < show.size(); i++) {
		int x = ((ll)i) % (show.size() - 1);
		
		int bestdiff = 0, besty;

		for (int j = 0; j < 100; j++) {

			int y = x + ((ll)rand()*rand()) % (show.size() - x - 1);

			if (x == y) continue;

			int diff = 0;

			if (x > 0) diff -= score(show[x-1], show[x]);
			if (x < show.size()-1) diff -= score(show[x], show[x+1]);
			if (y > 0) diff -= score(show[y-1], show[y]);
			if (y < show.size()-1) diff -= score(show[y], show[y+1]);

			auto old_x = show[x];
			auto old_y = show[y];

			// if (show[x].nd != -1 && show[y].nd != -1) {
			// 	if (rand() % 4 == 0) {
			// 		swap(show[x].st, show[y].st);
			// 	}
			// 	else if (rand() % 4 == 0) {
			// 		swap(show[x].st, show[y].nd);
			// 	}
			// }

			// if (x > 0) diff += score(show[x-1], show[y]);
			// if (x < show.size()-1) diff += score(show[y], show[x+1]);
			// if (y > 0) diff += score(show[y-1], show[x]);
			// if (y < show.size()-1) diff += score(show[x], show[y+1]);

			swap(show[x], show[y]);

			if (x > 0) diff += score(show[x-1], show[x]);
			if (x < show.size()-1) diff += score(show[x], show[x+1]);
			if (y > 0) diff += score(show[y-1], show[y]);
			if (y < show.size()-1) diff += score(show[y], show[y+1]);


			if (diff > bestdiff) {
				bestdiff = diff;
				besty = y;
			}
			
			show[x] = old_x;
			show[y] = old_y;
	

			// if (show[x].nd == -1 && show[y].nd == -1) {
			// 	if (x > 0) diff -= score(show[x-1], show[x]);
			// 	if (x < show.size()-1) diff -= score(show[x], show[x+1]);
			// 	if (y > 0) diff -= score(show[y-1], show[y]);
			// 	if (y < show.size()-1) diff -= score(show[y], show[y+1]);
			// }
			// else if(show[x].nd != -1 && show[y].nd != -1) {

			// }
		}

		if (bestdiff > 0) {
			total += bestdiff;
			swap(x, besty);
		}


		int per = int(double(i) / show.size() * 100);
		if (per > last) {
			cerr << per << "%" << " " << total << endl;
			last =per; 
		}
	}

	save(argv[1]);

	cout << show.size() << endl;
	for (auto slide: show) {
		if (slide.nd == -1) {
			cout << hor[slide.st].st << endl;
		}
		else {
			cout << ver[slide.st].st << " " << ver[slide.nd].st << endl;
		}
	}

	

	return 0;
}