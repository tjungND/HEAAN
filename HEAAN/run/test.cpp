/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/

#include "../src/HEAAN.h"
/**
  * This file is for test HEAAN library
  * You can find more in src/TestScheme.h
  * "./TestHEAAN Encrypt" will run Encrypt Test
  * There are Encrypt, EncryptSingle, Add, Mult, iMult, RotateFast, Conjugate Tests
  */
int main(int argc, char **argv) {
	long logq = 8001; ///< Ciphertext Modulus, only needed to change this for Taeho's experiment
    
    /*
     
     
     
     */
    
    
	long logp = 16; ///< Real message will be quantized by multiplying 2^(log p + 10)
	long logn = 1; ///< log2(The number of slots)
	long n = (1 << logn);
	srand(time(NULL));
    cout << endl;
	cout << "message precision = " << logp << endl;
    cout << "log(poly-degree, i.e., n) = " << logN << endl;
    cout << "log(temporary expansion) = " << logQ << endl;


    cout << "log(cipher modulus, i.e., q) = " << logq << endl;
	cout << "multiplicative depth = " << (logq / logp - 1) << endl;
	cout << "# of packed messages = " << n << endl;

    
    
	SetNumThreads(4);
	TimeUtils timeutils;
	Ring ring;
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);


//	timeutils.start("BootKeyGen (one-time)");
	scheme.addBootKey(secretKey, logn, logp + 10 + 4);
//	timeutils.stop("BootKeyGen (one-time)");

//----------------------------------------------------------------------------------
//   MULT TESTS
//----------------------------------------------------------------------------------

//	TestScheme::testMult(logq, logp, logn);





	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);


	Ciphertext* cipher = new Ciphertext[100];
	for(int i = 0; i < 100; i++){
		scheme.encrypt(cipher[i], mvec, n, logp, logq);
	}

/*
    
	cout << cipher[0].logp << endl;
	scheme.multAndEqual(cipher[0], cipher[1]);
	cout << cipher[0].logp << endl;
*/
	

	timeutils.start("Homomorphic Multiplication for 10 times");
//	timeutils.start("Homomorphic Multiplication");

	for(long i = 0; i < 10; i++){
        int j = rand() % 100;
        int k = rand() % 100;
        //cout << cipher[j].logp << endl;
		scheme.multAndEqual(cipher[j], cipher[k]);
        //cout << cipher[j].logp << endl;
	}
	timeutils.stop("Homomorphic Multiplication for 10 times");
//	timeutils.stop("Homomorphic Multiplication");
    
    cout << "Avg Mult. time is : " << (timeutils.timeElapsed / 10.0) << "ms" << endl;



//----------------------------------------------------------------------------------
//   BOOTSTRAPPING
//----------------------------------------------------------------------------------

    
    
    int depth = 0;
    int totalq= 0;
    
    logq = logp + 10; //< suppose the input ciphertext of bootstrapping has logq = logp + 10
//    	logn = 1; //< larger logn will make bootstrapping tech much slower
    long logT = 4; //< this means that we use Taylor approximation in [-1/T,1/T] with double angle fomula
    
    for(int i = 0; i < 100; i++){
        scheme.encrypt(cipher[i], mvec, n, logp, logq);
    }
    
	
//	cout << "cipher's logq before bootstrapping = " << cipher.logq << endl;
//	timeutils.start("Bootstrapping");
	timeutils.start("Bootstrapping for 5 times");
	
	for(long i = 0; i < 2; i++){
        int j = rand() % 100;
        //cout << "iteration " << i << endl;
        //cout << cipher[j].logq << endl;
		scheme.bootstrapAndEqual(cipher[j], logq, logQ, logT);
        //cout << cipher[j].logq << endl;
		depth += (cipher[j].logq / logp - 1);
		totalq += cipher[j].logq;
	}
	timeutils.stop("Bootstrapping for 5 times");
//	timeutils.stop("Bootstrapping");
    cout << "Avg Boot. time is : " << (timeutils.timeElapsed / 2.0) << "ms" << endl;
	cout << "avg depth = " << (depth / 2.0) << endl;
	cout << "avg logq = " << (totalq / 2.0) << endl;
    cout << endl;


//    	TestScheme::testBootstrap(logq, logp, logn, logT);

	return 0;
}
