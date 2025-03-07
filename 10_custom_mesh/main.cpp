#include <get_prolong.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  using namespace Eigen;
  using namespace std;

  // 命令行参数解析
  string input_mesh = "/Users/zane/Downloads/skirt.obj";
  string output_mesh = "/Users/zane/Downloads/decimated.obj";
  int target_faces = 1200;
  bool visualize = true;

  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-i" && i + 1 < argc) {
      input_mesh = argv[i + 1];
      i++;
    } else if (string(argv[i]) == "-o" && i + 1 < argc) {
      output_mesh = argv[i + 1];
      i++;
    } else if (string(argv[i]) == "-f" && i + 1 < argc) {
      target_faces = stoi(argv[i + 1]);
      i++;
    } else if (string(argv[i]) == "-nv") {
      visualize = false;
    } else if (string(argv[i]) == "-h") {
      cout << "用法: " << argv[0] << " [选项]" << endl
           << "选项:" << endl
           << "  -i <文件>     输入网格文件" << endl
           << "  -o <文件>     输出网格文件" << endl
           << "  -f <数量>     目标面片数量" << endl
           << "  -nv           不可视化结果" << endl
           << "  -h            显示帮助" << endl;
      return 0;
    }
  }

  // 加载输入网格
  MatrixXd V, VD;
  MatrixXi F, FD;

  cout << "正在加载网格: " << input_mesh << endl;
  if (!igl::read_triangle_mesh(input_mesh, V, F)) {
    cerr << "无法加载网格文件: " << input_mesh << endl;
    return 1;
  }
  cout << "原始网格: " << V.rows() << " 个顶点, " << F.rows() << " 个面片"
       << endl;

  // 执行顶点删除式降采样
  SparseMatrix<double> P;
  cout << "正在执行顶点删除式降采样..." << endl;
  cout << "目标面片数: " << target_faces << endl;

  get_prolong(V, F, target_faces, 2, VD, FD, P); // dec_type = 2 表示顶点删除

  cout << "降采样后: " << VD.rows() << " 个顶点, " << FD.rows() << " 个面片"
       << endl;

  // 保存结果
  cout << "正在保存结果到: " << output_mesh << endl;
  if (!igl::write_triangle_mesh(output_mesh, VD, FD)) {
    cerr << "无法保存网格文件: " << output_mesh << endl;
    return 1;
  }

  // 可视化结果
  if (visualize) {
    igl::opengl::glfw::Viewer viewer;

    // 添加原始网格和降采样网格
    viewer.data().set_mesh(V, F);
    viewer.append_mesh();
    viewer.data().set_mesh(VD, FD);

    // 设置颜色
    viewer.data(0).set_colors(RowVector3d(0.2, 0.8, 0.2)); // 原始网格为绿色
    viewer.data(1).set_colors(RowVector3d(0.8, 0.2, 0.2)); // 降采样网格为红色

    // 初始状态：同时显示两个网格
    viewer.data(0).set_visible(true);
    viewer.data(1).set_visible(true);

    // 显示网格
    viewer.callback_key_down = [&](igl::opengl::glfw::Viewer &,
                                   unsigned char key, int) -> bool {
      switch (key) {
      case '1':
        // 显示原始网格
        viewer.data(0).set_visible(true);
        viewer.data(1).set_visible(false);
        return true;
      case '2':
        // 显示降采样网格
        viewer.data(0).set_visible(false);
        viewer.data(1).set_visible(true);
        return true;
      case '3':
        // 同时显示两个网格
        viewer.data(0).set_visible(true);
        viewer.data(1).set_visible(true);
        return true;
      }
      return false;
    };

    cout << "可视化控制:" << endl
         << "  按 1: 显示原始网格" << endl
         << "  按 2: 显示降采样网格" << endl
         << "  按 3: 同时显示两个网格" << endl;

    viewer.launch();
  }

  return 0;
}
