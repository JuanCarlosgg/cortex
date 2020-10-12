
#include "dsr_camera_api.h"
#include "dsr_api.h"

using namespace DSR;

CameraAPI::CameraAPI(DSR::DSRGraph *G_, const DSR::Node &camera)
{
    G = G_;
    id = camera.agent_id();
    if( auto o_focal_x = G->get_attrib_by_name<cam_rgb_focalx_att>(camera); o_focal_x.has_value())
        focal_x = o_focal_x.value();
    if( auto o_focal_y = G->get_attrib_by_name<cam_rgb_focalx_att>(camera); o_focal_y.has_value())
        focal_x = o_focal_y.value();
    if( auto o_width = G->get_attrib_by_name<cam_rgb_width_att>(camera); o_width.has_value())
    {
        width = o_width.value();
        centre_x = width / 2;
    }
    if( auto o_height = G->get_attrib_by_name<cam_rgb_height_att>(camera); o_height.has_value())
    {
        height = o_height.value();
        centre_y = height/2;
    }
    if( auto o_depth = G->get_attrib_by_name<cam_rgb_depth_att>(camera); o_depth.has_value())
        depth = o_depth.value();
    if( auto o_id = G->get_attrib_by_name<cam_rgb_cameraID_att>(camera); o_id.has_value())
        depth = o_id.value();
}

Eigen::Vector2d CameraAPI::project( const Eigen::Vector3d & p) const
{
    Eigen::Vector2d proj;
    proj << focal_x * p.y() / p.x() + centre_x, focal_y * p.z() / p.x() + centre_y;
    return proj;
}

std::optional<std::reference_wrapper<const std::vector<uint8_t>>> CameraAPI::get_rgb_image() const
{
    if( const auto n = G->get_node(id); n.has_value())
    {
        auto &attrs = n.value().attrs();
        if (auto value = attrs.find("cam_rgb"); value != attrs.end())
            return value->second.byte_vec();
        else
        {
            qWarning() << __FUNCTION__ << "No rgb attribute found in node " << QString::fromStdString(n.value().name()) << ". Returning empty";
            return {};
        }
    }
    else
    {
        qWarning() << __FUNCTION__ << "No camera node found in G. Returning empty";
        return {};
    }
}
std::optional<std::vector<float>> CameraAPI::get_depth_image()
{
    if( const auto n = G->get_node(id); n.has_value())
    {
        auto &attrs = n.value().attrs();
        if (auto value = attrs.find("cam_depth"); value != attrs.end())
        {
            const std::size_t SIZE = value->second.byte_vec().size() / sizeof(float);
            float *depth_array = (float *) value->second.byte_vec().data();
            std::vector<float> res{depth_array, depth_array + SIZE};
            return res;
        } else
        {
            qWarning() << __FUNCTION__ << "No depth attribute found in node " << QString::fromStdString(n.value().name())
                       << ". Returning empty";
            return {};
        }
    }
    else
    {
        qWarning() << __FUNCTION__ << "No camera node found in G. Returning empty";
        return {};
    }
}
std::optional<std::reference_wrapper<const std::vector<uint8_t>>> CameraAPI::get_depth_image() const
{
    if( const auto n = G->get_node(id); n.has_value())
    {
        auto &attrs = n.value().attrs();
        if (auto value = attrs.find("cam_depth"); value != attrs.end())
        {
            return value->second.byte_vec();
        } else
        {
            qWarning() << __FUNCTION__ << "No depth attribute found in node " << QString::fromStdString(n.value().name())
                       << ". Returning empty";
            return {};
        }
    }
    else
    {
        qWarning() << __FUNCTION__ << "No camera node found in G. Returning empty";
        return {};
    }
}
std::optional<std::vector<std::tuple<float,float,float>>>  CameraAPI::get_pointcloud(const std::string target_frame_node, unsigned short subsampling)
{
    if( const auto n = G->get_node(id); n.has_value())
    {
        auto &attrs = n.value().attrs();
        if (auto value = attrs.find("cam_depth"); value != attrs.end())  //in metres
        {
            if (auto width = attrs.find("cam_depth_width"); width != attrs.end())
            {
                if (auto height = attrs.find("cam_depth_height"); height != attrs.end())
                {
                    if (auto focal = attrs.find("cam_depth_focalx"); focal != attrs.end())
                    {
                        const std::vector<uint8_t> &tmp = value->second.byte_vec();
                        if (subsampling == 0 or subsampling > tmp.size())
                        {
                            qWarning("DSRGraph::get_pointcloud: subsampling parameter < 1 or > than depth size");
                            return {};
                        }
                        // cast to float
                        float *depth_array = (float *) value->second.byte_vec().data();
                        const int WIDTH = width->second.dec();
                        const int HEIGHT = width->second.dec();
                        int FOCAL = focal->second.dec();
                        FOCAL = (int) ((WIDTH / 2) / atan(0.52));  // ÑAPA QUITAR
                        int STEP = subsampling;
                        float depth, X, Y, Z;
                        int cols, rows;
                        std::size_t SIZE = tmp.size() / sizeof(float);
                        std::vector<std::tuple<float, float, float>> result(SIZE);
                        std::unique_ptr<InnerEigenAPI> inner_eigen;
                        if (target_frame_node != "")  // do the change of coordinate system
                        {
                            inner_eigen = G->get_inner_eigen_api();
                            for (std::size_t i = 0; i < SIZE; i += STEP)
                            {
                                depth = depth_array[i];
                                cols = (i % WIDTH) - (WIDTH / 2);
                                rows = (HEIGHT / 2) - (i / WIDTH);
                                // compute axis coordinates according to the camera's coordinate system (Y outwards and Z up)
                                X = cols * depth / FOCAL * 1000;
                                Y = depth * 1000;
                                Z = rows * depth / FOCAL * 1000;
                                auto r = inner_eigen->transform(target_frame_node, Mat::Vector3d(X, Y, Z),
                                                                n.value().name()).value();
                                result[i] = std::make_tuple(r[0], r[1], r[2]);
                            }
                        } else
                            for (std::size_t i = 0; i < tmp.size() / STEP; i++)
                            {
                                depth = depth_array[i];
                                cols = (i % WIDTH) - 320;
                                rows = 240 - (i / WIDTH);
                                X = cols * depth / FOCAL * 1000;
                                Y = depth * 1000;
                                Z = rows * depth / FOCAL * 1000;
                                // we transform measurements to millimeters
                                result[i] = std::make_tuple(X, Y, Z);
                            }
                        return result;
                    } else
                    {
                        qWarning() << __FUNCTION__ << "No focal attribute found in node "
                                   << QString::fromStdString(n.value().name()) << ". Returning empty";
                        return {};
                    }
                } else
                {
                    qWarning() << __FUNCTION__ << "No HEIGHT attribute found in node "
                               << QString::fromStdString(n.value().name())
                               << ". Returning empty";
                    return {};
                }
            } else
            {
                qWarning() << __FUNCTION__ << "No WIDTH attribute found in node " << QString::fromStdString(n.value().name())
                           << ". Returning empty";
                return {};
            }
        } else
        {
            qWarning() << __FUNCTION__ << "No depth attribute found found in node " << QString::fromStdString(n.value().name())
                       << ". Returning empty";
            return {};
        }
    }
    else
    {
        qWarning() << __FUNCTION__ << "No camera node found in G. Returning empty";
        return {};
    }
}
std::optional<std::vector<uint8_t>> CameraAPI::get_depth_as_gray_image() const
{
    if( const auto n = G->get_node(id); n.has_value())
    {
        auto &attrs = n.value().attrs();
        if (auto value = attrs.find("cam_depth"); value != attrs.end())
        {
            const std::vector<uint8_t> &tmp = value->second.byte_vec();
            float *depth_array = (float *) value->second.byte_vec().data();
            const auto STEP = sizeof(float);
            std::vector<std::uint8_t> gray_image(tmp.size() / STEP);
            for (std::size_t i = 0; i < tmp.size() / STEP; i++)
                gray_image[i] = (int) (depth_array[i] * 15);  // ONLY VALID FOR SHORT RANGE, INDOOR SCENES
            return gray_image;
        } else
        {
            qWarning() << __FUNCTION__ << "No depth attribute found in node " << QString::fromStdString(n.value().name())
                       << ". Returning empty";
            return {};
        };
    }
    else
    {
        qWarning() << __FUNCTION__ << "No camera node found in G. Returning empty";
        return {};
    }
}
