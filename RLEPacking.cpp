struct Atlas {
    void Init(int w, int h) {
        W = w;
        H = h;
        Rows.resize(h, {-w});
        Columns.resize(w, {-h});
        CollapsedRows = {h};
        CollapsedColumns = {w};
    }

    void SetMap(std::vector<std::vector<int>> rows){
        Rows = rows;
        W = 0;

        for(int value : rows[0]){
            W+=(value<0 ? -value : value);
        }

        H = 0;
        bool isCollapsedColumnCounting = false;
        int columnCollapsedIterator = -1;
        bool isColumnFree = false;

        for(std::vector<int> row : rows){
            if((row[0] % W) == 0){
                if(isColumnFree != row[0]<0) isCollapsedColumnCounting = false;
                isColumnFree = row[0]<0;

                int uvalue = (row[0] / W)<0 ? -(row[0] / W) : row[0] / W;
                H += uvalue;

                if(!isCollapsedColumnCounting){
                    isCollapsedColumnCounting = true;
                    if(isColumnFree){
                        CollapsedColumns.push_back(-1);
                        columnCollapsedIterator++;
                    } else {
                        CollapsedColumns.push_back(1);
                        columnCollapsedIterator++;
                    }
                } else {
                    if(isColumnFree){
                        CollapsedColumns[columnCollapsedIterator]--;
                    } else {
                        CollapsedColumns[columnCollapsedIterator]++;
                    }
                }
            } else {
                H++;
                if(isCollapsedColumnCounting) isCollapsedColumnCounting = false;
            }
        }

        Columns.resize(W, {0});

        for(int row_n=0; row_n<rows.size(); row_n++){
            std::vector<int> row = rows[row_n];
            std::vector<int> lastRowPositiveSigns;

            if(row_n>0){
                for(int row_value : rows[row_n-1]){
                    for(int pos=0; pos<(row_value<0 ? -row_value : row_value); pos++){
                        lastRowPositiveSigns.push_back(row_value>0);
                    }
                }
            }

            int row_value_sum = 0;
            int row_value_last_sum = 0;

            for(int row_value_n=0; row_value_n<row.size(); row_value_n++){
                int row_value = row[row_value_n];
                row_value_sum += row_value<0 ? -row_value : row_value;

                for(int col_n=(row_value_last_sum<0 ? -row_value_last_sum : row_value_last_sum); col_n<row_value_sum; col_n++){
                    std::vector<int> column = Columns[col_n];

                    if(row_n>0 && ((!lastRowPositiveSigns[col_n] && row_value>0) || (lastRowPositiveSigns[col_n] && row_value<0))){
                        column.push_back(0);
                    }

                    int col_value_n = (column.size()-1)<0 ? 0 : column.size()-1;

                    if(row_value<0){
                        column[col_value_n] += -1;
                    } else {
                        column[col_value_n] += 1;
                    }

                    Columns[col_n] = column;
                }

                row_value_last_sum += row_value<0 ? -row_value : row_value;
            }
        }

        bool isCollapsedRowCounting = false;
        int rowCollapsedIterator = -1;
        bool isRowFree = false;

        for(std::vector<int> column : Columns){
            if((column[0] % H) == 0){
                if(isRowFree != column[0]<0) isCollapsedRowCounting = false;
                isRowFree = column[0]<0;

                if(!isCollapsedRowCounting){
                    isCollapsedRowCounting = true;
                    if(isRowFree){
                        CollapsedRows.push_back(-1);
                        rowCollapsedIterator++;
                    } else {
                        CollapsedRows.push_back(1);
                        rowCollapsedIterator++;
                    }
                } else {
                    if(isRowFree){
                        CollapsedRows[rowCollapsedIterator]--;
                    } else {
                        CollapsedRows[rowCollapsedIterator]++;
                    }
                }
            } else {
                if(isCollapsedRowCounting) isCollapsedRowCounting = false;
            }
        }
    }

    int W;
    int H;
    std::vector<std::vector<int>> Rows;
    std::vector<std::vector<int>> Columns;
    std::vector<int> CollapsedRows;
    std::vector<int> CollapsedColumns;
};

Atlas InsertRect(Atlas atlas, rbp::Rect rect){
    Atlas result = atlas;

    int h = rect.height;
    int w = rect.width;
    int x = rect.x;
    int y = rect.y;

    for(int i=0; i<h; i++){ // Перебрать строки
        int sum = 0;
        for(int j=0; j<w; j++){ // Перебрать элементы в строке
            sum += result.Rows[i+y][j];
            if(sum>=x){ // Если сумма в строке больше или равна позиции по горизонтали, то это наша позиция

            }
        }
    }

    return result;
}

struct Vec2i {
    int x;
    int y;
};

std::vector<Vec2i> GetAvailableSpaces(Atlas atlas, int w, int h)
{
    int W = atlas.W;
    int H = atlas.H;

    int row = 0;
    int collapsedColumnsIterator=0;

    bool isBreak = false;

    std::vector<Vec2i> result;

    for(; row<atlas.Rows.size(); row++){
        std::vector<int> Row = atlas.Rows[row];

        for(int nw=0, kw=0; nw<Row.size(); nw++){
            int value_w = Row[nw];
            int uvalue_w = value_w<0 ? -value_w : value_w;

            if(value_w <= -w){
                std::vector<int> Col = atlas.Columns[kw];

                for(int nh=0, kh=0; nh<Col.size(); nh++){
                    int value_h = Col[nh];
                    int uvalue_h = value_h<0 ? -value_h : value_h;

                    if(kh > row && value_h > 0) break;

                    if(value_h + (row - kh) <= -h){
                        std::vector<int> RightCol = atlas.Columns[(kw+w-1)];

                        for(int nhh=0, khh=0; nhh<RightCol.size(); nhh++){
                            int value_hh = RightCol[nhh];
                            int uvalue_hh = value_hh<0 ? -value_hh : value_hh;

                            if(khh > kh && value_hh > 0) break;

                            if(khh >= kh && (value_hh + (row - kh)) <= -h){
                                std::vector<int> BottomRow = atlas.Rows[(kh+h-1)];

                                for(int nww=0, kww=0; nww<BottomRow.size(); nww++){
                                    int value_ww = BottomRow[nww];
                                    int uvalue_ww = value_ww<0 ? -value_ww : value_ww;

                                    if(kww > kw && value_ww > 0) break;

                                    if(kww >= kw && value_ww <= -w){
                                        result.push_back(Vec2i{kw, row});
                                    }
                                    kww += uvalue_ww;
                                }
                            }
                            khh += uvalue_hh;
                        }
                    }
                    kh += uvalue_h;
                }
            }
            kw += uvalue_w;
        }

        if(atlas.Rows[row][0] % W == 0){
            row+=atlas.CollapsedColumns[collapsedColumnsIterator]-1;
            collapsedColumnsIterator++;
        }
    }

    return result;
}
