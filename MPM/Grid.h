#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>
#include "Particle.h"
#include "Scene.h"
#include "Util.h"
#include "SimConstant.h"

class Grid {
	std::vector<EulerianNode> eulerianNodes;
	
	Vector2f size;//width and height;
	float interval_x, interval_y;
	int x_num, y_num;
#if ENABLE_IMPLICIT
	std::vector<Matrix2f> Hessian;//(d^2 * e^n)/(dx_i * dx_j), is a symmetric matirx
	float rAr, ApAp, rAr_next;
#endif
public:
	Grid() = default;
	Grid(float _interval_x, float _interval_y, Vector2f _size = Vector2f(1.0f, 1.0f));
	Grid(int resolution_x, int resolution_y, Vector2f _size = Vector2f(1.0f, 1.0f));

	void PtoG(Scene &scene);
	void updateGridVelocity();
	void explicitVelocity() {
		for (int i = 0, length = eulerianNodes.size(); i < length; ++i) {
			eulerianNodes[i].velocity /= eulerianNodes[i].mass;
			eulerianNodes[i].velocity += TIMESTEP * (GRAVITY + eulerianNodes[i].explicitForce / eulerianNodes[i].mass);
		}
	}
#if ENABLE_IMPLICIT
	void initHessian();
	void implicitVelocity();
#endif
	void collision();
	void reset();
	Matrix2f getHessian(const int &i, const int &j) const;

	void setHessian(const int &i, const int &j, const Matrix2f &m);

	float getIntervalX() { return interval_x; }
	float getIntervalY() { return interval_y; }
	EulerianNode& getNode(int index) { return eulerianNodes[index]; }
	int getX_num() { return x_num; }
	int getY_num() { return y_num; }
};

#endif // !GRID_H

//void Grid::initHessian(){
//	int nodes_num = EulerGrid.size();
//	for (int i = 0; i < nodes_num; i++) {
//		for (int j = 0; j <= i; j++) {
//			
//
//		}
//	}
//}
//
//void Grid::implicitVelocity() {
//	int nodes_num = EulerGrid.size();
//	float total_residual_len = 0.0;
//	initHessian();
//	rAr = 0.0;
//	ApAp = 0.0;
//	/*-----initial guess-----*/
//	for (int i = 0; i < nodes_num; i++) {
//		if (EulerGrid[i].active) {
//			EulerGrid[i].imp_active = true;
//
//			EulerGrid[i].r = Vector2f::Zero();
//			for (int j = 0; j < nodes_num; j++) {
//				if (EulerGrid[j].active) {
//					EulerGrid[i].r += getHessian(i, j) * EulerGrid[j].velocity;
//				}
//			}
//			EulerGrid[i].r = EulerGrid[i].velocity - EulerGrid[i].r;//r0 = b - A * x;
//
//			EulerGrid[i].Ar = Vector2f::Zero();
//			for (int j = 0; j < nodes_num; j++) {
//				if (EulerGrid[j].active) {
//					EulerGrid[i].Ar += getHessian(i, j) * EulerGrid[j].r;
//				}
//			}
//
//			EulerGrid[i].p = EulerGrid[i].r;//p0 = r0
//			EulerGrid[i].Ap = EulerGrid[i].Ar;//Ap0 = Ar0
//
//			rAr += EulerGrid[i].r.dot(EulerGrid[i].Ar);
//			ApAp += EulerGrid[i].Ap.length_squared();
//		}
//	}
//	/*-----loop-----*/
//	for (int iter = 0; iter < MAX_IMPLICT_ITER; iter++) {
//		total_residual_len = 0.0;
//		/*-----compute alpha_k-----*/
//		float alpha = rAr / ApAp;
//		/*-----compute v_k+1, r_k+1-----*/
//		for (int i = 0; i < nodes_num; i++) {
//			if (EulerGrid[i].imp_active) {
//				EulerGrid[i].velocity += alpha * EulerGrid[i].p;
//				EulerGrid[i].r_next = EulerGrid[i].r - alpha * EulerGrid[i].Ap;
//				
//				float residual_len = EulerGrid[i].r_next.length();
//				if (residual_len < MIN_RESIDUAL || residual_len > MAX_RESIDUAL || isnan(residual_len)) {
//					EulerGrid[i].imp_active = false;
//				}
//				else {
//					total_residual_len += residual_len * residual_len;
//				}
//			}
//		}
//		if (sqrt(total_residual_len) < MIN_RESIDUAL) break;//residual is small enough, so the iteration should end up.
//
//		/*-----compute Ar_k+1-----*/
//		rAr_next = 0;
//		for (int i = 0; i < nodes_num; i++) {
//			if (EulerGrid[i].imp_active) {
//				EulerGrid[i].Ar_next = Vector2f::Zero();
//				for (int j = 0; j < nodes_num; j++) {
//					if (EulerGrid[j].imp_active) {
//						EulerGrid[i].Ar_next += getHessian(i, j) * EulerGrid[j].r_next;
//					}
//				}
//				rAr_next += EulerGrid[i].r_next.dot(EulerGrid[i].Ar_next);
//			}
//		}
//		/*-----compute beta_k-----*/
//		float beta = rAr_next / rAr;
//		/*-----compute p_k+1, Ap_k+1, and update r_k, p_k, Ap_k, ApAp to prepare for the next iteration-----*/
//		ApAp = 0;
//		for (int i = 0; i < nodes_num; i++) {
//			if (EulerGrid[i].imp_active) {
//				EulerGrid[i].p = EulerGrid[i].r_next + beta * EulerGrid[i].p;
//				EulerGrid[i].Ap = EulerGrid[i].Ar_next + beta * EulerGrid[i].Ap;
//				
//				EulerGrid[i].r = EulerGrid[i].r_next;
//				EulerGrid[i].Ar = EulerGrid[i].Ar_next;
//				ApAp += EulerGrid[i].Ap.length_squared();
//			}
//		}
//		/*-----update rAr to prepare for the next iteration-----*/
//		rAr = rAr_next;
//	}
//}