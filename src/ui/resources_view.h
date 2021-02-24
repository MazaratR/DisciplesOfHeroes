#ifndef RESOURCES_VIEW_H
#define RESOURCES_VIEW_H

#include <QWidget>
#include "resource_types.h"

namespace Ui {
class ResourcesView;
}

class ResourcesView : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcesView(QWidget *parent = nullptr);
    ~ResourcesView();

    void updateResourceAmount(const ResourceMap& _map);

private:
    Ui::ResourcesView *ui;
};

#endif // RESOURCES_VIEW_H
