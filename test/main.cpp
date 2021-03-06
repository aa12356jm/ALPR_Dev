#include "easypr.h"
#include "easypr/util/switch.hpp"

#include "accuracy.hpp"
#include "chars.hpp"
#include "plate.hpp"

// %OPENCV%\x86\vc12\lib opencv_world300d.lib;

namespace easypr {

namespace demo {
// interactions
//批量测试函数
int accuracyTestMain() {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  kv->load("../etc/chinese_mapping");

  bool isExit = false;
  while (!isExit) {
    easypr::Utils::print_file_lines("../etc/batch_test_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    Result result;

    while (isRepeat) {
      std::cin >> select;
      isRepeat = false;
      switch (select) {
        case 1:
          accuracyTest("resources/image/general_test", result);
          break;
        case 2:
          accuracyTest("resources/image/native_test", result);
          break;
        case 3:
          gridSearchTest("resources/image/general_test");
          break;
        case 4:
          isExit = true;
          break;
        default:
          std::cout << kv->get("input_error") << ":";
          isRepeat = true;
          break;
      }
    }
  }
  return 0;
}

//测试函数，可以进行一系列测试，车牌检测，字符识别等等
int testMain() 
{
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  //加载文件中的内容，以键值对的形式保存
  kv->load("../etc/chinese_mapping");

  bool isExit = false;
  while (!isExit) {
    Utils::print_file_lines("../etc/test_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    while (isRepeat) 
	{
      std::cin >> select;
      isRepeat = false;
      switch (select) {
        case 1:
			//在一幅图像中定位出可能是车牌的矩形区域，显示输出
          assert(test_plate_locate() == 0);
          break;
        case 2:
			//车牌判断，判断图像是否是车牌
          assert(test_plate_judge() == 0);
          break;
        case 3:
			//检测包含定位和判断，使用三种方式定位（sobel,color,cmser），使用nms进行判断
          assert(test_plate_detect() == 0);
          break;
        case 4:
			//将车牌中的每个字符分割出来并显示
          assert(test_chars_segment() == 0);
          break;
        case 5:
			//将车牌区域内的字符都检测并显示出来,和正确字符做对比是否正确
			assert(test_chars_identify() == 0);
          break;
        case 6:
			//将车牌区域中所有车牌字符检测并显示出来
          assert(test_chars_recognise() == 0);
          break;
        case 7:
			//车牌检测函数，输入一副包含车牌的图像，在图像中将检测到的车牌区域划线表示
          assert(test_plate_recognize() == 0);
          break;
        case 8:
			//执行上面所有步骤
          assert(test_plate_locate() == 0);
          assert(test_plate_judge() == 0);
          assert(test_plate_detect() == 0);

          assert(test_chars_segment() == 0);
		  assert(test_chars_identify() == 0);
          assert(test_chars_recognise() == 0);

          assert(test_plate_recognize() == 0);
          break;
        case 9:
          isExit = true;
          break;
        default:
          std::cout << kv->get("input_error") << ":";
          isRepeat = true;
          break;
      }
    }
  }

  return 0;
}

}  // namespace demo

}  // namespace easypr

//处理命令行信息
void command_line_handler(int argc, const char* argv[]) {
  program_options::Generator options;

  options.add_subroutine("svm", "svm operations").make_usage("Usage:");
  {
    /* ------------------------------------------
     | SVM Training operations
     | ------------------------------------------
     |
     | $ demo svm --plates=path/to/plates/ [--test] --svm=save/to/svm.xml
     |
     | ------------------------------------------
     */
    options("h,help", "show help information");
    options(",plates", "",
            "a folder contains both forward data and inverse data in the "
                "separated subfolders");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
    options("t,test", "run tests in --plates");
  }

  options.add_subroutine("ann", "ann operation").make_usage("Usages:");
  {
    /* ------------------------------------------
    | ANN_MLP Training operations
    | ------------------------------------------
    |
    | $ demo ann --zh-chars=zhchars/ --en-chars=enchars/ --ann=save/to/ann.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options(",chars", "",
            "the folder contains character sub-folders, with each folder"
                "named by label defined in include/easypr/config.h");
    options(",ann", easypr::kDefaultAnnPath,
            "the ann model file you want to save");
    options("t,test", "run test in --chars");
  }

  options.add_subroutine("locate", "locate plates in an image")
      .make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate locating operations
    | ------------------------------------------
    |
    | $ demo locate -f file
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("f,file", "",
            "the target picture which contains one or more plates");
  }

  options.add_subroutine(
          "judge", "determine whether an image block is the license plate")
      .make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate judge operations
    | ------------------------------------------
    |
    | $ demo judge -f file --svm resources/model/svm.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("f,file", "the target image block");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
  }

  options.add_subroutine("recognize", "plate recognition").make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate recognize operations
    | ------------------------------------------
    |
    | $ demo recognize -p file --svm resources/model/svm.xml
    |                          --ann resources/model/ann.xml
    | $ demo recognize -pb dir/ --svm resources/model/svm.xml
    |                           --ann resources/model/ann.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("p,path", "", "where is the target picture or target folder");
    options("b,batch", "do batch recognition, if set, --path means a folder");
    options("c,color", "returns the plate color, blue or yellow");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
    options(",ann", easypr::kDefaultAnnPath, "the ann model file");
  }

  auto parser = options.make_parser();

  try {
    parser->parse(argc, argv);
  } catch (const std::exception &err) {
    std::cout << err.what() << std::endl;
    return;
  }

  auto subname = parser->get_subroutine_name();

  program_options::select(subname)
      .found("svm",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("svm");
                 return;
               }

               easypr::SvmTrain svm(parser->get("plates")->c_str(),
                                    parser->get("svm")->c_str());

               if (parser->has("test")) {
                 svm.test();
               } else {
                 svm.train();
               }
             })
      .found("ann",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("ann");
                 return;
               }

               assert(parser->has("chars"));
               assert(parser->has("ann"));

               easypr::AnnTrain ann(parser->get("chars")->c_str(),
                                    parser->get("ann")->c_str());

               if (parser->has("test")) {
                 ann.test();
               } else {
                 ann.train();
               }
             })
      .found("locate",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("locate");
                 return;
               }

               if (parser->has("file")) {
                 easypr::api::plate_locate(parser->get("file")->val().c_str());
                 std::cout << "finished, results can be found in tmp/"
                     << std::endl;
               }
             })
      .found("judge",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("judge");
                 std::cout << "Note that the input image's size should "
                     << "be the same as the one you gived to svm train."
                     << std::endl;
                 return;
               }

               if (parser->has("file")) {
                 assert(parser->has("file"));
                 assert(parser->has("svm"));

                 auto image = parser->get("file")->val();
                 auto svm = parser->get("svm")->val();

                 const char* true_or_false[2] = {"false", "true"};

                 std::cout << true_or_false[easypr::api::plate_judge(
                     image.c_str(), svm.c_str())]
                     << std::endl;
               }
             })
      .found("recognize",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("recognize");
                 return;
               }

               if (parser->has("path")) {
                 if (parser->has("batch")) {
                   // batch testing
                   auto folder = parser->get("path")->val();
                   easypr::demo::Result result;
                   easypr::demo::accuracyTest(folder.c_str(), result);
                 } else {
                   // single testing
                   auto image = parser->get("path")->val();

                   if (parser->has("color")) {
                     // return plate color
                     const char* colors[2] = {"blue", "yellow"};
                     std::cout
                         << colors[easypr::api::get_plate_color(image.c_str())]
                         << std::endl;
                   } else {
                     // return strings
                     auto svm = parser->get("svm")->val();
                     auto ann = parser->get("ann")->val();

                     auto results = easypr::api::plate_recognize(
                         image.c_str(), svm.c_str(), ann.c_str());
                     for (auto s : results) {
                       std::cout << s << std::endl;
                     }
                   }
                 }
               } else {
                 std::cout << "option 'file' cannot be empty." << std::endl;
               }
             })
      .others([&]() {
        // no case matched, print all commands.
        std::cout << "There are several sub commands listed below, "
            << "choose one by typing:\n\n"
            << "    " << easypr::utils::getFileName(argv[0])
            << " command [options]\n\n"
            << "The commands are:\n" << std::endl;
        auto subs = options.get_subroutine_list();
        for (auto sub : subs) {
          fprintf(stdout, "%s    %s\n", sub.first.c_str(), sub.second.c_str());
        }
        std::cout << std::endl;
      });
}

//主函数
int main(int argc, const char* argv[]) {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);//智能指针kv，指向easypr::Kv类
  kv->load("../etc/chinese_mapping");//加载此文件中内容到kv类的成员变量vector类型的data_中

  if (argc > 1) 
  {
    // handle command line execution.
    command_line_handler(argc, argv);
    return 0;
  }

  bool isExit = false;
  while (!isExit) 
  {
	  //一行一行读取菜单内容并显示到dos中
    easypr::Utils::print_file_lines("../etc/main_menu");
	//get函数显示文件中make_a_choice对应的值，键值对的形式保存
	std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;

    while (isRepeat) 
	{
      std::cin >> select;//等待输入数字，选择对应的选项
      isRepeat = false;
      switch (select) 
	  {
        case 1:
          easypr::demo::testMain();//测试
          break;
        case 2:
          easypr::demo::accuracyTestMain();//批量测试
          break;
        case 3://SVM训练
          std::cout << "Run \"demo svm\" for more usage." << std::endl;
          {
            easypr::SvmTrain svm("../resources/train/svm", "../tmp/svm.xml");
            svm.train();
          }
          break;
        case 4://ANN训练
          std::cout << "Run \"demo ann\" for more usage." << std::endl;
          {
            easypr::AnnTrain ann("../resources/train/ann", "../tmp/ann.xml");
            ann.train();
          }
          break;
        case 5://GDTS生成
          easypr::preprocess::generate_gdts();
          break;
        case 6://开发团队
		{
          easypr::Utils::print_file_lines("../etc/dev_team");
          break;
        }
        case 7://感谢名单 
		{
          easypr::Utils::print_file_lines("../etc/thanks");
          break;
        }
        case 8://退出
          isExit = true;
          break;
        default:
          std::cout << kv->get("input_error") << ":";
          isRepeat = true;
          break;
      }
    }
  }
  return 0;
}