#include "Particle.h"
#include "SimConstant.h"
#include "Util.h"

//void LargrangianParticle::SVD_Decomposition() {
//	//SVD Decomposition
//	JacobiSVD<Matrix2f> SVD(F_Elastic, Eigen::ComputeFullU | Eigen::ComputeFullV);
//	SVD_Singular(0, 0) = SVD.singularValues()(0);
//	SVD_Singular(1, 1) = SVD.singularValues()(1);
//	SVD_U = SVD.matrixU();
//	SVD_V = SVD.matrixV();
//}

void LargrangianParticle::computeSigma() {
	Matrix2f R_Elastic = SVD_U * SVD_V.transpose();//Polar decomposition
	float J_Elastic = F_Elastic.determinant(), J_Plastic = F_Plastic.determinant();
	float harden = exp(HARDENING * (1 - J_Plastic));
	
	Sigma = volume * harden * (2 * mu * (F_Elastic - R_Elastic) * F_Elastic.transpose() + lambda * (J_Elastic - 1) * J_Elastic * Matrix2f::Identity());
	/*-----debug-----*/
	int stop = 1;
	/*-----debug-----*/
}


void LargrangianParticle::updateF(){
	F_Elastic = (Matrix2f::Identity() + TIMESTEP * velocity_gradient) * F_Elastic;//temp new F_Elastic

	Matrix2f temp_new_F = F_Elastic * F_Plastic;

	F_Elastic.svd_decomposition(SVD_U, SVD_Singular, SVD_V);
	clamp(SVD_Singular[0][0], 1 - THETA_C, 1 + THETA_S);
	clamp(SVD_Singular[1][1], 1 - THETA_C, 1 + THETA_S);

	Matrix2f inverse_SVD_Singular = SVD_Singular;
	inverse_SVD_Singular[0][0] = 1.0 / inverse_SVD_Singular[0][0];
	inverse_SVD_Singular[1][1] = 1.0 / inverse_SVD_Singular[1][1];

	F_Elastic = SVD_U * SVD_Singular * SVD_V.transpose();
	F_Plastic = SVD_V * inverse_SVD_Singular * SVD_U.transpose() * temp_new_F;
}