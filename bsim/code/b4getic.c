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

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "bsim4def.h"
#include "sperror.h"
#include "suffix.h"

int
BSIM4getic(inModel,ckt)
GENmodel *inModel;
CKTcircuit *ckt;
{
BSIM4model *model = (BSIM4model*)inModel;
BSIM4instance *here;

    for (; model ; model = model->BSIM4nextModel)
    {    for (here = model->BSIM4instances; here; here = here->BSIM4nextInstance)
     {    if (!here->BSIM4icVDSGiven)
          {   here->BSIM4icVDS = *(ckt->CKTrhs + here->BSIM4dNode)
                   - *(ckt->CKTrhs + here->BSIM4sNode);
              }
              if (!here->BSIM4icVGSGiven)
          {   here->BSIM4icVGS = *(ckt->CKTrhs + here->BSIM4gNodeExt)
                   - *(ckt->CKTrhs + here->BSIM4sNode);
              }
              if(!here->BSIM4icVBSGiven)
              {  here->BSIM4icVBS = *(ckt->CKTrhs + here->BSIM4bNode)
                                  - *(ckt->CKTrhs + here->BSIM4sNode);
              }
         }
    }
    return(OK);
}
