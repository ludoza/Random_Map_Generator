#include "mainwindow.h"
#include "flare/Map.h"
#include "MapGenerator.h"
#include "TilesetDef.h"
#include "ui_mainwindow.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map_pointer            = new Map;
    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_C1;
    app_data.file_name     = "default.txt";
    app_data.gen_chests    = false;
    app_data.gen_enemies   = false;
    app_data.gen_exits     = true;
    app_data.progress      = 0;
    app_data.size_x        = 100;
    app_data.size_y        = 100;
    app_data.seed          = time(NULL);
    app_data.seed_set      = false;
    app_data.status        = "Ready";
    app_data.tile_set      = "cave";

    std::vector<std::string> tileset_names = TilesetDef::tilesetNames();
    for (unsigned i=0; i<tileset_names.size(); ++i) {
        ui->comboBox_tileSet->insertItem(i, QString(tileset_names[i].c_str()));
    }
}

MainWindow::~MainWindow()
{
    delete   map_pointer;
    delete ui;
}

void MainWindow::on_comboBox_algorithm_currentIndexChanged(int index)
{
    MainWindow::app_data.algorithm = index;
}

void MainWindow::on_comboBox_tileSet_currentIndexChanged(int index)
{
    MainWindow::app_data.tile_set = ui->comboBox_tileSet->itemText(index).toStdString();
}

void MainWindow::on_comboBox_mapSize_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
        default:
        MainWindow::app_data.size_x = 100;
        MainWindow::app_data.size_y = 100;
        break;
    }
}

void MainWindow::on_checkBox_genExits_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_exits = false;
    else MainWindow::app_data.gen_exits = true;
}

void MainWindow::on_checkBox_genChests_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_chests = false;
    else MainWindow::app_data.gen_chests = true;
}

void MainWindow::on_checkBox_genEnemies_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_enemies = false;
    else MainWindow::app_data.gen_enemies = true;
}

void MainWindow::on_lineEdit_seed_textChanged(const QString &arg1)
{
    app_data.seed_set = true;
    MainWindow::app_data.seed = arg1.toInt();
    if (arg1.isEmpty())
        app_data.seed_set = false;
}

void MainWindow::on_pushButton_released()
{
    app_data.file_name = MainWindow::ui->lineEdit->text().toStdString();
    this->ui->progressBar->setValue(0);
    this->ui->label_status->setText("Seeding random...");
    this->ui->progressBar->setValue(1);
    this->ui->label_status->setText("Initializing intermediate map...");
    this->ui->progressBar->setValue(10);
    this->ui->label_status->setText("Generating map...");

    MapProperties properties;
    properties.algorithm = static_cast<Algorithm_Type::Algorithm_Type>(app_data.algorithm);
    
    properties.tile_set = app_data.tile_set;
    properties.size_x = app_data.size_x;
    properties.size_y = app_data.size_y;
    properties.seed = app_data.seed;
    properties.seed_set = app_data.seed_set;
    properties.gen_exits = app_data.gen_exits;

    MapGenerate(map_pointer, properties);

    this->ui->textEdit_mapview->clear();

    renderPreview(map_pointer);

    this->ui->progressBar->setValue(80);
    this->ui->label_status->setText("Applying tile set...");
    this->ui->progressBar->setValue(90);
    this->ui->label_status->setText("Exporting to file...");
    MapGenerator::Export(map_pointer, app_data.file_name);
    this->ui->progressBar->setValue(100);
    this->ui->label_status->setText("Done.");
}

void MainWindow::renderPreview(Map* map_pointer)
{
    std::stringstream temp_stream;
    int intermediate = -1;

    for (unsigned i = 0; i < map_pointer->layers.size(); ++i) {
            if (map_pointer->layernames[i] == "intermediate") {
                intermediate = i;
            }
    }
    if (intermediate == -1) return;

    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            if (map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_FLOOR) temp_stream << "<font color=#006600>";
            if (map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_WALL)  temp_stream << "<font color=#F8F8F8>";
            if (map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_EXIT)  temp_stream << "<font color=#FF0000>";
            temp_stream << "&#9632;";//map_pointer->tile[i].data;
            temp_stream << "</font>";
            if ((i % map_pointer->w) == (map_pointer->w-1)) temp_stream << std::endl;
        }
    }
    this->ui->textEdit_mapview->insertHtml(temp_stream.str().c_str());
    
    // this code is here to make preview work. Correct way is to have this in MapGenerator::applyTileset()
    delete [] map_pointer->layers[intermediate];
    map_pointer->layers.erase(map_pointer->layers.begin() + intermediate);
    map_pointer->layernames.erase(map_pointer->layernames.begin() + intermediate);
}
