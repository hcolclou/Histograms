
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* node constructor given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),LT(NULL),RB(NULL)
	{}

void twoDtree::Node::clear() {
    LT->clear();
    RB->clear();
    delete LT;
    delete RB;
}

twoDtree::Node* twoDtree::Node::copy() {
    twoDtree::Node* newNode = new twoDtree::Node(upLeft, lowRight, avg);
    if (LT != NULL) {
        newNode->LT = LT->copy();
    }
    if (RB != NULL) {
        newNode->RB = RB->copy();
    }
    return newNode;
}

bool twoDtree::Node::allInTol(HSLAPixel main, double tol) {
    if (LT == NULL && RB == NULL) {
        return main.dist(avg) < tol;
    } else {
        bool ret = true;
        if (LT != NULL) {
            ret &= LT->allInTol(avg, tol);
        }
        if (RT != NULL) {
            ret &= RB->allInTol(avg, tol);
        }
    }
}

void twoDtree::Node::prune(double tol) {
    if (LT != NULL && RB != NULL) {
        if (allInTol(avg, tol) {
            clear();
        } else {
            if (LT != NULL) {
                LT->prune(tol);
            }
            if (RB != NULL) {
                RB->prune(tol);
            }
        }
    }
}

void twoDtree::Node::render(PNG& im) {
    if (LT != NULL && RB != NULL) {
        LT->render(im);
        RB->render(im);
    } else {
        for (unsigned int x = upLeft.first; x < lowRight.first; x++) {
            for (unsigned int y = upLeft.second; y < lowRight.second; y++) {
                *(im->getPixel(x, y)) = avg;
            }
        }
    }
}

/* destructor given */
twoDtree::~twoDtree(){
	clear();
}

/* copy constructor given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


/* operator= given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/* twoDtree constructor */
twoDtree::twoDtree(PNG & imIn){
    stats s(imIn);
    root = buildTree(s, make_pair(0, 0), make_pair(imIn.width() - 1, imIn.height() - 1), true);
}

/* buildTree helper for twoDtree constructor */
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
    Node* newNode = new Node(ul, lr, s.getAvg(ul, lr));
    int height = lr.second - ul.second + 1;
    int width = lr.first - ul.first + 1;

    if (height > 1 || width > 1) {
        int offset;
        if (vert || width == 1) {
            vector<double> dist;

            for (unsigned int y = ul.second; y < lr.second - 1; y++) {
                pair<int, int> middle = make_pair(lr.first, y);
                pair<int, int> middlep1 = make_pair(ul.first, y + 1);
                dist.push_back(s.rectArea(ul, middle) * s.entropy(ul, middle)
                    + s.rectArea(middlep1, lr) * s.entropy(middlep1, lr));
            }

            double min = min_element(dist.begin(), dist.end());
            offset = find(dist.rbegin(), dist.rend(), min);

            newNode->LT = buildTree(s, ul, make_pair(lr.first, ul.second + offset), !vert);
            newNode->RB = buildTree(s, make_pair(ul.first, ul.second + offset + 1), lr, !vert);
        } else {
            vector<double> dist;

            for (unsigned int x = ul.first; x < lr.first - 1; x++) {
                pair<int, int> middle = make_pair(x, ul.second);
                pair<int, int> middlep1 = make_pair(x + 1, lr.second);
                dist.push_back(s.rectArea(ul, middle) * s.entropy(ul, middle)
                    + s.rectArea(middlep1, lr) * s.entropy(middlep1, lr));
            }

            double min = min_element(dist.begin(), dist.end());
            offset = find(dist.rbegin(), dist.rend(), min);

            newNode->LT = buildTree(s, ul, make_pair(ul.first + offset, lr.second), !vert);
            newNode->RB = buildTree(s, make_pair(ul.first + offset + 1, ul.second), lr, !vert);
        }
    }

    return newNode;
}

/* render your twoDtree into a png */
PNG twoDtree::render(){
    PNG ret(width, height);
    root->render(ret);
    return ret;
}

/* prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of
 * the average pixel value contained in the root
 * of the subtree
 */
void twoDtree::prune(double tol){
    root->prune(tol);
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the twoDtree */
void twoDtree::clear() {
    root->clear();
    delete root;
}


/* helper function for copy constructor and op= */
void twoDtree::copy(const twoDtree & orig){
    width = orig.width;
    height = orig.height;
    if (orig.root != NULL) {
        root = orig.root->copy();

    }
}
