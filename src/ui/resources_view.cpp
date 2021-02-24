#include "resources_view.h"
#include "ui_resources_view.h"

ResourcesView::ResourcesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResourcesView)
{
    ui->setupUi(this);
}

ResourcesView::~ResourcesView()
{
    delete ui;
}

void ResourcesView::updateResourceAmount(const ResourceMap &_map)
{
    auto updateResourceAmount = [&_map](QSpinBox* _sb, ResourceType _type)
    {
        auto it = _map.find(_type);
        int amount = 0;
        if (it != _map.end())
            amount = it->second;

        _sb->setValue(amount);
    };

    updateResourceAmount(ui->sb_goldAmount,  ResourceType::Gold);
    updateResourceAmount(ui->sb_gemAmount,   ResourceType::Gem);
    updateResourceAmount(ui->sb_woodAmount,  ResourceType::Wood);
    updateResourceAmount(ui->sb_stoneAmount, ResourceType::Stone);
}
