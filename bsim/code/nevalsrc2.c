/* ******************************************************************************
   *  BSIM4 4.8.2 released by Chetan Kumar Dabhi 01/01/2020                     *
   *  BSIM4 Model Equations                                                     *
   ******************************************************************************

   ******************************************************************************
   *  Copyright (c) 2020 University of California                               *
   *                                                                            *
   *  Project Director: Prof. Chenming Hu.                                      *
   *  Current developers: Chetan Kumar Dabhi   (Ph.D. student, IIT Kanpur)      *
   *                      Prof. Yogesh Chauhan (IIT Kanpur)                     *
   *                      Dr. Pragya Kushwaha  (Postdoc, UC Berkeley)           *
   *                      Dr. Avirup Dasgupta  (Postdoc, UC Berkeley)           *
   *                      Ming-Yen Kao         (Ph.D. student, UC Berkeley)     *
   *  Authors: Gary W. Ng, Weidong Liu, Xuemei Xi, Mohan Dunga, Wenwei Yang     *
   *           Ali Niknejad, Chetan Kumar Dabhi, Yogesh Singh Chauhan,          *
   *           Sayeef Salahuddin, Chenming Hu                                   * 
   ******************************************************************************/

/*
Licensed under Educational Community License, Version 2.0 (the "License"); you may
not use this file except in compliance with the License. You may obtain a copy of the license at
http://opensource.org/licenses/ECL-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations
under the License.

BSIM-CMG model is supported by the members of Silicon Integration Initiative's Compact Model Coalition. A link to the most recent version of this
standard can be found at: http://www.si2.org/cmc 
*/


/*
 * NevalSrc2 (noise, lnNoise, ckt, type, node1, node2, param, node3, node4, param2)
 *   This routine is a modified version of NevalSrc() that computes
 *   the output noise due to two fully-correlated noise sources. It is
 *   useful for implementing correlated gate and drain noises in MOSFETs.
 *
 *   Like NevalSrc(), it evaluates the noise due to different physical
 *   phenomena.  This includes the "shot" noise associated with dc
 *   currents in semiconductors and the "thermal" noise associated with
 *   resistance.  Although semiconductors also display "flicker" (1/f)
 *   noise, the lack of a unified model requires us to handle it on a
 *   "case by case" basis.  What we CAN provide, though, is the noise
 *   gain associated with the 1/f source.
 */
/* Modified by Darsen Lu for BSIM4 tnoiMod=2 10/10/2010
*/

#include "spice.h"
#include "cktdefs.h"
#include "const.h"
#include "noisedef.h"
#include "util.h"
#include "suffix.h"

void
NevalSrc2 (noise, lnNoise, ckt, type, node1, node2, param1, node3, node4, param2, phi21)

double *noise;
double *lnNoise;
CKTcircuit *ckt;
int type;
int node1;
int node2;
double param1;
int node3;
int node4;
double param2;
double phi21;     /* Phase of signal 2 relative to signal 1 */

{
    double realVal1, imagVal1;
    double realVal2, imagVal2;
    double realOut, imagOut, param_gain;
    double T0, T1, T2, T3;

    realVal1 = *((ckt->CKTrhs) + node1) - *((ckt->CKTrhs) + node2);
    imagVal1 = *((ckt->CKTirhs) + node1) - *((ckt->CKTirhs) + node2);
    realVal2 = *((ckt->CKTrhs) + node3) - *((ckt->CKTrhs) + node4);
    imagVal2 = *((ckt->CKTirhs) + node3) - *((ckt->CKTirhs) + node4);
    T0 = sqrt(param1);
    T1 = sqrt(param2);
    T2 = T1 * cos(phi21);
    T3 = T1 * sin(phi21);
    realOut = T0 * realVal1 + T2 * realVal2 - T3 * imagVal2;
    imagOut = T0 * imagVal1 + T2 * imagVal2 + T3 * realVal2;
    param_gain = (realOut*realOut) + (imagOut*imagOut);
    switch (type) {

    case SHOTNOISE:
    *noise = 2.0 * CHARGE * FABS(param_gain);          /* param is the dc current in a semiconductor */
    *lnNoise = log( MAX(*noise,N_MINLOG) );
    break;

    case THERMNOISE:
    *noise = 4.0 * CONSTboltz * ckt->CKTtemp * param_gain;         /* param is the conductance of a resistor */
        *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case N_GAIN:
    *noise = 0.0;
    break;

    }
}
