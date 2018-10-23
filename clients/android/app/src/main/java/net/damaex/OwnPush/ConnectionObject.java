package net.damaex.OwnPush;

class ConnectionObject {
    public enum Purpose {
        ERROR(-1),
        REQUEST(0),
        CHALLENGE(1),
        LOGIN(2),
        PUSH(3),
        RESET(4);

        private final int type;

        Purpose(int type) {
            this.type = type;
        }

        public int getType() {
            return this.type;
        }
    }

    ConnectionObject() {
        this.purpose = Purpose.ERROR;
        this.data = "";
    }

    ConnectionObject(Purpose purpose, String data) {
        this.purpose = purpose;
        this.data = data;
    }

    Purpose purpose;
    String data;
}
