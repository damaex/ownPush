using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ownPush
{
    enum Purpose
    {
        REQUEST = 0,
        CHALLENGE = 1,
        LOGIN = 2
    }

    class ConnectionObject
    {
        public ConnectionObject(Purpose pur, string d)
        {
            purpose = pur;
            data = d;
        }

        public Purpose purpose;
        public string data;
    }
}
