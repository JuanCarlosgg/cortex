//
// Created by juancarlos on 1/6/21.
//

#include<dsr/core/types/common_types.h>


namespace DSR {

    /////////////////////////////////////////////////////
    /// Attribute
    ////////////////////////////////////////////////////

    IDL::Attrib Attribute::to_IDL_attrib() {
        IDL::Attrib att;
        att.timestamp(m_timestamp);
        att.type(m_value.index());
        att.value(to_IDL_val());
        att.agent_id(m_agent_id);
        return att;
    }

    IDL::Val Attribute::to_IDL_val()
    {
        IDL::Val value;

        switch (m_value.index()) {
            case 0:
                value.str(std::get<std::string>(m_value));
                break;
            case 1:
                value.dec(std::get<int32_t>(m_value));
                break;
            case 2:
                value.fl(std::get<float>(m_value));
                break;
            case 3:
                value.float_vec(std::get<std::vector<float>>(m_value));
                break;
            case 4:
                value.bl(std::get<bool>(m_value));
                break;
            case 5:
                value.byte_vec(std::get<std::vector<uint8_t>>(m_value));
                break;
            case 6:
                value.uint(std::get<std::uint32_t>(m_value));
                break;
            case 7:
                value.u64(std::get<std::uint64_t>(m_value));
                break;
            case 8:
                value.dob(std::get<double>(m_value));
                break;
            case 9:
                value.uint64_vec(std::get<std::vector<uint64_t>>(m_value));
                break;
            case 10:
                value.vec_float2(std::get<std::array<float, 2>>(m_value));
                break;
            case 11:
                value.vec_float3(std::get<std::array<float, 3>>(m_value));
                break;
            case 12:
                value.vec_float4(std::get<std::array<float, 4>>(m_value));
                break;
            case 13:
                value.vec_float6(std::get<std::array<float, 6>>(m_value));
                break;
            default:
                throw std::runtime_error(
                        ("Error converting DSR::Attribute to IDL::Attrib. The Attribute is uninitialized. " +
                         std::to_string(__LINE__) + " " + __FILE__).data());
        }

        return value;
    }

    const ValType &Attribute::value() const
    {
        return m_value;
    }

    ValType& Attribute::value()
    {
        return m_value;
    }

    uint64_t Attribute::timestamp() const
    {
        return m_timestamp;
    }

    uint32_t Attribute::agent_id() const
    {
        return m_agent_id;
    }

    void Attribute::timestamp(uint64_t t)
    {
        m_timestamp = t;
    }

    void Attribute::value(const ValType &value)
    {
        m_value = value;
    }

    void Attribute::value(ValType &&value)
    {
        m_value = std::move(value);
    }

    void Attribute::agent_id(uint32_t agentId)
    {
        m_agent_id = agentId;
    }


    [[nodiscard]] std::size_t Attribute::selected() const
    {
        return m_value.index();
    }

    std::string &Attribute::str()
    {
        if (auto pval = std::get_if<std::string>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("STRING is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    [[nodiscard]] const std::string &Attribute::str() const
    {
        if (auto pval = std::get_if<std::string>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("STRING is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    void Attribute::str(const std::string &str)
    {
        m_value = str;
    }

    void Attribute::str(std::string &&str)
    {
        m_value = std::move(str);
    }

    void Attribute::dec(int32_t dec)
    {
        m_value = dec;
    }

    [[nodiscard]] int32_t Attribute::dec() const
    {
        if (auto pval = std::get_if<int32_t>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("INT is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    void Attribute::uint(uint32_t uint)
    {
        m_value = uint;
    }

    [[nodiscard]] uint32_t Attribute::uint() const
    {
        if (auto pval = std::get_if<uint32_t>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("UINT is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    void Attribute::uint64(uint64_t uint)
    {
        m_value = uint;
    }

    [[nodiscard]] uint64_t Attribute::uint64() const
    {
        if (auto pval = std::get_if<uint64_t>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("UINT64 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    void Attribute::fl(float fl)
    {
        m_value = fl;
    }

    [[nodiscard]] float Attribute::fl() const
    {
        if (auto pval = std::get_if<float>(&m_value)) {
            return *pval;
        }

        throw std::runtime_error(
                ("FLOAT is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    void Attribute::dob(double dob)
    {
        m_value = dob;
    }

    [[nodiscard]] double Attribute::dob() const
    {
        if (auto pval = std::get_if<double>(&m_value)) {
            return *pval;
        }

        throw std::runtime_error(
                ("DOUBLE is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    void Attribute::float_vec(const std::vector<float> &float_vec)
    {
        m_value = float_vec;
    }

    void Attribute::float_vec(std::vector<float> &&float_vec)
    {
        m_value = std::move(float_vec);
    }

    const std::vector<float> &Attribute::float_vec() const
    {
        if (auto pval = std::get_if<std::vector<float>>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VECTOR_FLOAT is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    std::vector<float> &Attribute::float_vec()
    {

        if (auto pval = std::get_if<std::vector<float>>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VECTOR_FLOAT is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    void Attribute::bl(bool bl)
    {
        m_value = bl;
    }

    [[nodiscard]] bool Attribute::bl() const
    {

        if (auto pval = std::get_if<bool>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("BOOL is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    void Attribute::byte_vec(const std::vector<uint8_t> &float_vec)
    {
        m_value = float_vec;
    }

    void Attribute::byte_vec(std::vector<uint8_t> &&float_vec)
    {
        m_value = std::move(float_vec);
    }

    [[nodiscard]] const std::vector<uint8_t> &Attribute::byte_vec() const
    {
        if (auto pval = std::get_if<std::vector<uint8_t>>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VECTOR_BYTE is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }

    std::vector<uint8_t> &Attribute::byte_vec()
    {

        if (auto pval = std::get_if<std::vector<uint8_t >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VECTOR_BYTE is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }



    void Attribute::u64_vec(const std::vector<uint64_t> &uint64_vec)
    {
        m_value = uint64_vec;
    }

    void Attribute::u64_vec(std::vector<uint64_t> &&uint64_vec)
    {
        m_value = std::move(uint64_vec);
    }

    [[nodiscard]] const std::vector<uint64_t> &Attribute::u64_vec() const
    {
        if (auto pval = std::get_if<std::vector<uint64_t >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("U64_VEC is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    std::vector<uint64_t> &Attribute::u64_vec()
    {
        if (auto pval = std::get_if<std::vector<uint64_t >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("U64_VEC is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());
    }



    void Attribute::vec2(const std::array<float, 2> &vec_float2)
    {
        m_value = vec_float2;
    }

    [[nodiscard]] const std::array<float, 2> &Attribute::vec2() const
    {
        if (auto pval = std::get_if<std::array<float, 2 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC2 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    std::array<float, 2> &Attribute::vec2()
    {
        if (auto pval = std::get_if<std::array<float, 2 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC2 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }



    void Attribute::vec3(const std::array<float, 3> &vec_float3)
    {
        m_value = vec_float3;
    }

    [[nodiscard]] const std::array<float, 3> &Attribute::vec3() const
    {
        if (auto pval = std::get_if<std::array<float, 3 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC3 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

    std::array<float, 3> &Attribute::vec3()
    {
        if (auto pval = std::get_if<std::array<float, 3 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC3 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());


    }


    void Attribute::vec4(const std::array<float, 4> &vec_float4)
    {
        m_value = vec_float4;
    }

    [[nodiscard]] const std::array<float, 4> &Attribute::vec4() const
    {
        if (auto pval = std::get_if<std::array<float, 4 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC4 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());


    }

    std::array<float, 4> &Attribute::vec4()
    {
        if (auto pval = std::get_if<std::array<float, 4 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC4 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());


    }


    void Attribute::vec6(const std::array<float, 6> &vec_float6)
    {
        m_value = vec_float6;
    }


    [[nodiscard]] const std::array<float, 6> &Attribute::vec6() const
    {
        if (auto pval = std::get_if<std::array<float, 6 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC6 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());


    }

    std::array<float, 6> &Attribute::vec6()
    {
        if (auto pval = std::get_if<std::array<float, 6 >>(&m_value)) {
            return *pval;
        }
        throw std::runtime_error(
                ("VEC6 is not selected, selected is " + std::string(TYPENAMES_UNION[m_value.index()])).data());

    }

}