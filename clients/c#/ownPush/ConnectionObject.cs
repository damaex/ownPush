namespace ownPush
{
    enum Purpose
    {
        ERROR = -1,
        REQUEST = 0,
        CHALLENGE = 1,
        LOGIN = 2,
        PUSH = 3,
        RESET = 4
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

        public override string ToString()
        {
            return "Purpose: " + purpose + " | " + data;
        }
    }
}
