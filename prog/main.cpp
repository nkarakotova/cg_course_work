#if defined(TEST)

#include <gtest/gtest.h>
#include "tests.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else

#include "mainwindow.hpp"

bool enteredParametrs(MainWindow &w, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (!enteredParametrs(w, argc, argv))
        return EXIT_SUCCESS;

    w.show();
    return a.exec();
}

bool enteredParametrs(MainWindow &w, int argc, char *argv[])
{
    if (argc == 4 && !strcmp(argv[1], "-image"))
    {
        w.setScene(argv[2]);
        w.getImage(argv[3] + std::string(".png"));
        return EXIT_SUCCESS;
    }

    if (argc == 5 && !strcmp(argv[1], "-film"))
    {
        w.setScene(argv[2]);
        w.getImage(argv[3] + std::string("_0.png"));

        std::ifstream commands(argv[4]);

        if (!commands.is_open())
            return EXIT_FAILURE;

        std::string id;
        char action;
        double x, y, z;

        int i = 1;

        while (commands >> id >> action >> x >> y >> z)
        {
            int int_id = stoi(id);

            switch (action)
            {
                case ('m'):
                    w.moveObject(int_id, x, y, z);
                    break;
                case ('s'):
                    w.scaleObject(int_id, x);
                    break;
                case ('r'):
                    w.rotateObject(int_id, x, y, z);
                    break;
                default:
                    return EXIT_FAILURE;
            }
            
            w.getImage(argv[3] + std::string("_") + std::to_string(i) + std::string(".png"));
            i++;
        }

        return EXIT_SUCCESS;
    }

    if (argc == 5 && !strcmp(argv[1], "-research"))
    {
        w.setScene(argv[2]);
        int t = 0;

        std::string depth = argv[4];

        for (int i = 0; i < 10; i++)
            t += w.getImage(argv[3] + std::string(".png"), stoi(depth));

        float res = (float)t * 60 * 60 / CLOCKS_PER_SEC / 10;

        std::cout << depth << " " << std::fixed << res << "\n";

        return EXIT_SUCCESS;
    }

   return EXIT_FAILURE;
}

#endif
