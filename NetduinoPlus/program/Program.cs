// This software is based on the source code of SDK Fingerprint Demo of Beijing Smackbio Technology Co., Ltd
// and is published under the GPL license because there is no prior indication of this.
// Copyright (C) Alvaro Fernandez

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using SecretLabs.NETMF.Hardware;
using SecretLabs.NETMF.Hardware.NetduinoPlus;

namespace program
{
    public class Program
    {
        const byte CMD_ACK = 0x30;

        static CGT511C3 fpUnit;

        public static void Main()
        {
            fpUnit = new CGT511C3(1, 9600);

            //------- 1) Get num fingerprints -----------
            //int numFP = GetNumFingerprints();

            //------- 2) Led On               -----------
            //LedOn();

            //------- 3) Led Off              -----------    
            //LedOff();

            //------- 4) Enroll Fingerprint   -----------
            //EnrollFingerprint(3);

            //------- 5) Identify User        -----------
            //int iTmp = Identify();

            //------- 6) Verify User          -----------
            // Introduce the User Id to Verify
            //int id = 1;
            //if (Verify(id))
            //{
                //User correctly verified
            //}
                //User rejected

            //------- 7) IsPressFinger        -----------
            //bool iFinger = IsPressFinger();

            //------- 8) Delete All Fingerprints
            //DeleteAll();
        }

        static int GetNumFingerprints()
        {
            fpUnit.getEnrollCount();
            return fpUnit.gwLastAckParam;
        }

        static void LedOn()
        {
            fpUnit.cmosLed(true);
        }

        static void LedOff()
        {
            fpUnit.cmosLed(false);
        }

        static bool EnrollFingerprint(int ID)
        {
            fpUnit.cmosLed(true);
            fpUnit.enroll_start(ID);

            //cout << "Put your finger in the module and Press any key to Capture the fingerprint (1/3)" << endl;
            //system("pause > nul");

            fpUnit.captureFinger(true);
            fpUnit.enroll_num(1);

            //cout << "Put your finger in the module and Press any key to Capture the fingerprint (2/3)" << endl;
            //system("pause > nul");

            fpUnit.captureFinger(true);
            fpUnit.enroll_num(2);

            //cout << "Put your finger in the module and Press any key to Capture the fingerprint (3/3)" << endl;
            //system("pause > nul");

            fpUnit.captureFinger(true);
            fpUnit.enroll_num(3);

            fpUnit.cmosLed(false);

            //cout << "Ready." << endl;
            return true;
        }

        static int Identify()
        {
            int response;

            fpUnit.cmosLed(true);

            //cout << endl;
            //cout << "Put your finger in the module and Press any key to Continue" << endl;
            //system("pause > nul");

            fpUnit.captureFinger(true);
            fpUnit.identify();
            if (fpUnit.gwLastAck == CMD_ACK)
                response = fpUnit.gwLastAckParam;
            else
                response = -1;
            fpUnit.cmosLed(false);

            return response;
        }

        static bool Verify(int nId)
        {
            bool response = false;

            fpUnit.cmosLed(true);

            //printf("Put your finger in the module and Press any key to continue\n");
            //system("pause > nul");

            fpUnit.captureFinger(true);
            fpUnit.verify(nId);
            if (fpUnit.gwLastAck == CMD_ACK)
                response = true;
            fpUnit.cmosLed(false);

            return response;
        }

        static bool IsPressFinger()
        {
            fpUnit.cmosLed(true);

            //cout << endl;
            //cout << "Put your finger in the module and Press any key to Continue" << endl;
            //system("pause > nul");

            fpUnit.isPressFinger();
            if (fpUnit.gwLastAckParam == 0)
            {
                fpUnit.cmosLed(false);
                return true;
            }
            fpUnit.cmosLed(false);
            return false;
        }

        static bool DeleteAll()
        {
            fpUnit.deleteAll();
            if (fpUnit.gwLastAckParam == 0)
                return true;
            return false;
        }
    }
}
