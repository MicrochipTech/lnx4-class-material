/*
 * Copyright (C) 2021 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <egt/ui>

int main(int argc, char** argv)
{
        egt::Application app(argc, argv);

        egt::TopWindow window;
        egt::Button button(window, "Press Me");
        egt::center(button);
        window.show();


        return app.run();
}

